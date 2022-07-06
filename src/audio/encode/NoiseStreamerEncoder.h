#ifndef NoiseStreamerEncoder_h__
#define NoiseStreamerEncoder_h__

#include "liblame/LibLame.h"

class NoiseStreamerEncoder
{
private:
    LibLame* lame;

#ifdef HAVE_LAME
    hip_t hip;
    mp3data_struct mp3data;
#endif

public:
    NoiseStreamerEncoder();
    virtual ~NoiseStreamerEncoder();

    void initForDecode();
    int decode(unsigned char* mp3Buffer, size_t mp3Len, short pcmLeft[], short pcmRight[]);
    void finilizeDecode();

    void initForEncode();
    void encode();
    void finilizeEncode();
};

#endif // NoiseStreamerEncoder_h__
