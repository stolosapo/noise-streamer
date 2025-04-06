#ifndef DecodeMP3_h__
#define DecodeMP3_h__

#include "../CircularBuffer.h"

#include <noisekernel/Signal.h>

using namespace NoiseKernel;

void decode_mp3(const char* input_file, CircularBuffer* output_buffer, SignalAdapter* sigSrv, long& rate, int& channels, int& encoding);

#endif // DecodeMP3_h__
