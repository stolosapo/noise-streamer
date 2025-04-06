#ifndef CircularBuffer_h__
#define CircularBuffer_h__

#include <noisekernel/Thread.h>
#include <noisekernel/Signal.h>

using namespace NoiseKernel;

class CircularBuffer
{
private:
    short* buffer;
    size_t buffer_size;
    size_t read_pos;
    size_t write_pos;
    bool full;

    pthread_mutex_t mutex;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;

    SignalAdapter* sigAdapter;

    bool is_full() const;

public:
    CircularBuffer(SignalAdapter* sigAdapter, size_t size);
    ~CircularBuffer();

    void write(const short* data, size_t num_samples);
    void read(short* data, size_t num_samples);
    bool is_empty() const;
    void reset();
    
};

#endif // CircularBuffer_h__
