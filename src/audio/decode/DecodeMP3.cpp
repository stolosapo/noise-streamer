#include "DecodeMP3.h"
#include <iostream>
#include <mpg123.h>

#include "../BufferSizes.h"

using namespace std;

void decode_mp3(const char* input_file, CircularBuffer* output_buffer, SignalAdapter* sigSrv, long& rate, int& channels, int& encoding) 
{
    mpg123_handle* mh;
    int err;
    size_t done;

    // Initialize mpg123
    mpg123_init();
    mh = mpg123_new(nullptr, &err);
    if (!mh) 
    {
        cerr << "Unable to create mpg123 handle: " << mpg123_plain_strerror(err) << endl;
        return;
    }

    // Open the MP3 file
    if (mpg123_open(mh, input_file) != MPG123_OK) 
    {
        cerr << "Unable to open MP3 file: " << mpg123_strerror(mh) << endl;
        mpg123_delete(mh);
        mpg123_exit();
        return;
    }

    // Get audio format information
    mpg123_getformat(mh, &rate, &channels, &encoding);

    // Ensure the output format is signed 16-bit PCM
    mpg123_format_none(mh);
    mpg123_format(mh, rate, channels, MPG123_ENC_SIGNED_16);

    // Buffer for decoded PCM data
    size_t buffer_size = mpg123_outblock(mh);
    // const size_t buffer_size = MP3_DECODE_BUFFER;
    short pcm_buffer[buffer_size];

    // Decode the MP3 file
    while (!sigSrv->gotSigInt() && mpg123_read(mh, reinterpret_cast<unsigned char*>(pcm_buffer), buffer_size * sizeof(short), &done) == MPG123_OK) 
    {
        size_t num_samples = done / sizeof(short);
        output_buffer->write(pcm_buffer, num_samples); // Write PCM data to the circular buffer

        // cout << "(DecodeMP3) pcm_buffer: " << pcm_buffer << " num_samples: " << num_samples << endl;
    }

    // Cleanup
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();
}