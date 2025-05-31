#ifndef AudioDecoder_h__
#define AudioDecoder_h__

#include <noisekernel/Signal.h>

#include "../CircularBuffer.h"
#include "../TrackMetadata.h"

using namespace NoiseKernel;

class AudioDecoder
{
protected:
    SignalAdapter* sigSrv;
    CircularBuffer<short>* outputBuffer;

public:
    AudioDecoder(SignalAdapter* sigSrv, CircularBuffer<short>* outputBuffer);
    virtual ~AudioDecoder();

    virtual bool open(const char* filename) = 0;
    virtual void close() = 0;
    virtual bool decode() = 0;
    virtual int getSampleRate() const = 0;
    virtual int getChannels() const = 0;
    virtual int64_t getTotalSamples() const = 0;
    virtual bool getMetadata(TrackMetadata &metadata) const = 0;
};

#endif // AudioDecoder_h__
