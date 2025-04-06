#ifndef BufferSizes_h__
#define BufferSizes_h__

#include <iostream>

using namespace std;

const size_t MP3_DECODE_BUFFER = 8192;      // mpg123_read()

const size_t PCM_BUFFER_SAMPLES = 4096;     // CircularBuffer (stereo 16-bit)

const size_t RESAMPLE_CHUNK = 1024;         // libsamplerate

const size_t LAME_ENCODE_BUFFER = 8192;     // lame_encode_buffer()

const size_t SHOUT_SEND_CHUNK = 8192;       // shout_send()

#endif // BufferSizes_h__