#include "AudioDecoder.h"

AudioDecoder::AudioDecoder(SignalAdapter* sigSrv, CircularBuffer<short>* outputBuffer)
    : sigSrv(sigSrv),
    outputBuffer(outputBuffer)
{

}

AudioDecoder::~AudioDecoder()
{

}