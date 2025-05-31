#ifndef MP3Decoder_h__
#define MP3Decoder_h__

#include "AudioDecoder.h"

// #ifdef HAVE_MPG123
    #include <mpg123.h>
// #endif

class MP3Decoder: public AudioDecoder
{
private:
// #ifdef HAVE_MPG123
    mpg123_handle* mh;
// #endif

public:
    MP3Decoder(SignalAdapter* sigSrv, CircularBuffer<short>* outputBuffer);
    virtual ~MP3Decoder();

    virtual bool open(const char* filename);
    virtual void close();
    virtual bool decode();
    virtual int getSampleRate() const;
    virtual int getChannels() const;
    virtual int64_t getTotalSamples() const;
    virtual bool getMetadata(TrackMetadata &metadata) const;
};

#endif // MP3Decoder_h__
