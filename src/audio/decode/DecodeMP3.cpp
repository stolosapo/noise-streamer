#include "DecodeMP3.h"
#include <iostream>
#include <mpg123.h>

#include "../BufferSizes.h"

using namespace std;

bool decode_mp3(const char* input_file, CircularBuffer<short>* output_buffer, SignalAdapter* sigSrv, long& rate, int& channels, int& encoding)
{
    mpg123_handle* mh = NULL;
    int err = MPG123_OK;
    
    // Initialize the library
    err = mpg123_init();
    if (err != MPG123_OK)
    {
        std::cerr << "mpg123_init failed: " << mpg123_plain_strerror(err) << std::endl;
        return false;
    }
    
    // Create a new handle
    mh = mpg123_new(NULL, &err);
    if (!mh)
    {
        std::cerr << "mpg123_new failed: " << mpg123_plain_strerror(err) << std::endl;
        mpg123_exit();
        return false;
    }
    
    // Open the file
    if (mpg123_open(mh, input_file) != MPG123_OK)
    {
        std::cerr << "mpg123_open failed: " << mpg123_strerror(mh) << std::endl;
        mpg123_delete(mh);
        mpg123_exit();
        return false;
    }
    
    // Get format information
    if (mpg123_getformat(mh, &rate, &channels, &encoding) != MPG123_OK)
    {
        std::cerr << "mpg123_getformat failed: " << mpg123_strerror(mh) << std::endl;
        mpg123_close(mh);
        mpg123_delete(mh);
        mpg123_exit();
        return false;
    }
    
    // Ensure output format is signed 16-bit integers
    mpg123_format_none(mh);
    if (mpg123_format(mh, rate, channels, MPG123_ENC_SIGNED_16) != MPG123_OK)
    {
        std::cerr << "mpg123_format failed: " << mpg123_strerror(mh) << std::endl;
        mpg123_close(mh);
        mpg123_delete(mh);
        mpg123_exit();
        return false;
    }

    // cout << "SampleRate: " << rate << ", Channels: " << channels << ", Encoding: " << encoding << endl;
    
    // Prepare buffer and read loop
    size_t buffer_size = mpg123_outblock(mh);
    std::vector<unsigned char> read_buffer(buffer_size);
    size_t done = 0;
    
    // Read and decode the entire file
    do
    {
        err = mpg123_read(mh, &read_buffer[0], read_buffer.size(), &done);
        
        if (done > 0)
        {
            // Convert to short* (16-bit samples) and write to circular buffer
            size_t sample_count = done / sizeof(short);
            if (!output_buffer->write(reinterpret_cast<short*>(&read_buffer[0]), sample_count))
            {
                // Buffer was closed
                break;
            }
        }
    } while (err == MPG123_OK && done > 0);
    
    if (err != MPG123_DONE && err != MPG123_OK)
    {
        std::cerr << "mpg123_read failed: " << mpg123_strerror(mh) << std::endl;
        mpg123_close(mh);
        mpg123_delete(mh);
        mpg123_exit();
        return false;
    }
    
    // Cleanup
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();
    
    return true;
}