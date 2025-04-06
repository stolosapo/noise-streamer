#include "CircularBuffer.h"
#include <iostream>

using namespace std;

CircularBuffer::CircularBuffer(SignalAdapter* sigAdapter, size_t size)
    : sigAdapter(sigAdapter), buffer(new short[size]), buffer_size(size), read_pos(0), write_pos(0), full(false) 
{
    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&not_full, nullptr);
    pthread_cond_init(&not_empty, nullptr);
}

CircularBuffer::~CircularBuffer() 
{
    delete[] buffer;
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&not_full);
    pthread_cond_destroy(&not_empty);
}

void CircularBuffer::reset()
{
    pthread_cond_signal(&not_full);
    pthread_cond_signal(&not_empty);
}

bool CircularBuffer::is_full() const 
{
    return full;
}

bool CircularBuffer::is_empty() const 
{
    return !full && (read_pos == write_pos);
}

void CircularBuffer::write(const short* data, size_t num_samples) 
{
    pthread_mutex_lock(&mutex);

    // Wait until there is space in the buffer
    while (!sigAdapter->gotSigInt() && is_full()) 
    {
        pthread_cond_wait(&not_full, &mutex);
    }

    // Write data to the buffer
    for (size_t i = 0; i < num_samples; ++i) 
    {
        buffer[write_pos] = data[i];
        write_pos = (write_pos + 1) % buffer_size;
        if (write_pos == read_pos) 
        {
            full = true;
        }
    }

    // Signal that the buffer is no longer empty
    pthread_cond_signal(&not_empty);
    pthread_mutex_unlock(&mutex);
}

void CircularBuffer::read(short* data, size_t num_samples) 
{
    pthread_mutex_lock(&mutex);

    // Wait until there is data in the buffer
    while (!sigAdapter->gotSigInt() && is_empty()) 
    {
        pthread_cond_wait(&not_empty, &mutex);
    }

    // Read data from the buffer
    for (size_t i = 0; i < num_samples; ++i) 
    {
        data[i] = buffer[read_pos];
        read_pos = (read_pos + 1) % buffer_size;
        full = false;
    }

    // Signal that the buffer is no longer full
    pthread_cond_signal(&not_full);
    pthread_mutex_unlock(&mutex);
}
