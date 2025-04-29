#ifndef CircularBuffer_v2_h__
#define CircularBuffer_v2_h__

#include <noisekernel/Thread.h>
#include <noisekernel/Signal.h>

using namespace NoiseKernel;

template <typename T>
class CircularBuffer_v2
{
private:
    SignalAdapter* sigAdapter;

    std::vector<T> buffer;
    size_t capacity;
    size_t read_pos;
    size_t write_pos;
    size_t available;
    pthread_mutex_t mutex;
    pthread_cond_t cond_producer;
    pthread_cond_t cond_consumer;
    bool is_closed;

    // Prevent copying
    CircularBuffer_v2(const CircularBuffer_v2&);
    CircularBuffer_v2& operator=(const CircularBuffer_v2&);

public:
    CircularBuffer_v2(SignalAdapter* sigAdapter, size_t capacity);
    virtual ~CircularBuffer_v2();

    bool write(const T* data, size_t count);
    size_t read(T* data, size_t count);
    void close();
    bool isClosed() const;
};

template <typename T>
CircularBuffer_v2<T>::CircularBuffer_v2(SignalAdapter* sigAdapter, size_t capacity) 
    : sigAdapter(sigAdapter),
    buffer(capacity), 
    capacity(capacity), 
    read_pos(0), 
    write_pos(0),
    available(0), 
    is_closed(false) 
{
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_producer, NULL);
    pthread_cond_init(&cond_consumer, NULL);
}

template <typename T>
CircularBuffer_v2<T>::~CircularBuffer_v2() 
{
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_producer);
    pthread_cond_destroy(&cond_consumer);
}

template <typename T>
bool CircularBuffer_v2<T>::write(const T* data, size_t count) 
{
    pthread_mutex_lock(&mutex);
    
    if (is_closed) 
    {
        pthread_mutex_unlock(&mutex);
        return false;
    }
    
    // Wait until there's enough space
    while (!this->sigAdapter->gotSigInt() && (capacity - available) < count && !is_closed) 
    {
        pthread_cond_wait(&cond_producer, &mutex);
    }

    if (is_closed) 
    {
        pthread_mutex_unlock(&mutex);
        return false;
    }

    // Write data
    for (size_t i = 0; i < count; ++i) 
    {
        buffer[write_pos] = data[i];
        write_pos = (write_pos + 1) % capacity;
    }

    available += count;
    pthread_cond_signal(&cond_consumer);
    pthread_mutex_unlock(&mutex);
    return true;
}

template <typename T>
size_t CircularBuffer_v2<T>::read(T* data, size_t count) 
{
    pthread_mutex_lock(&mutex);
    
    // Wait until there's enough data or buffer is closed
    while (!this->sigAdapter->gotSigInt() && available == 0 && !is_closed) 
    {
        pthread_cond_wait(&cond_consumer, &mutex);
    }

    if (available == 0 && is_closed) 
    {
        pthread_mutex_unlock(&mutex);
        return 0;
    }

    // Read as much as we can (up to count)
    size_t to_read = count;
    if (to_read > available) 
    {
        to_read = available;
    }
    
    for (size_t i = 0; i < to_read; ++i) 
    {
        data[i] = buffer[read_pos];
        read_pos = (read_pos + 1) % capacity;
    }

    available -= to_read;
    pthread_cond_signal(&cond_producer);
    pthread_mutex_unlock(&mutex);
    return to_read;
}

template <typename T>
void CircularBuffer_v2<T>::close() 
{
    pthread_mutex_lock(&mutex);
    is_closed = true;
    pthread_cond_broadcast(&cond_producer);
    pthread_cond_broadcast(&cond_consumer);
    pthread_mutex_unlock(&mutex);
}

template <typename T>
bool CircularBuffer_v2<T>::isClosed() const 
{
    return is_closed;
}

#endif // CircularBuffer_v2_h__
