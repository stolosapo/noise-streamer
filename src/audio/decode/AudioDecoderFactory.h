#ifndef AudioDecoderFactory_h__
#define AudioDecoderFactory_h__

#include <string>
#include "AudioDecoder.h"

using namespace std;

AudioDecoder* createDecoder(const char* filename, SignalAdapter* sigSrv, CircularBuffer<short>* outputBuffer);

#endif // AudioDecoderFactory_h__
