#ifndef DecodeMP3_h__
#define DecodeMP3_h__

#include "../CircularBuffer.h"

#include <noisekernel/Signal.h>

using namespace NoiseKernel;

bool decode_mp3(const char* input_file, CircularBuffer<short>* output_buffer, SignalAdapter* sigSrv, long& rate, int& channels, int& encoding);

#endif // DecodeMP3_h__
