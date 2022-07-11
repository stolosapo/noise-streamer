#ifndef NoiseStreamerEncoder_h__
#define NoiseStreamerEncoder_h__

#include "liblame/LibLame.h"
#include "../tag/AudioTag.h"
#include "EncodeContext.h"

class NoiseStreamerEncoder
{
private:
    LibLame* lame;

#ifdef HAVE_LAME
    hip_t hip;
    mp3data_struct mp3data;
#endif

    void setId3Tag(AudioTag* settings);
    void setChannels(int channels);
    void setVbrTag(EncodeContext *context);
    void setEncodeModeABR(int bitrate);
    void setEncodeModeCBR(int quality, int bitrate);
    void setEncodeModeVBR(int quality);

public:
    static const int MP3_SIZE;
    static const int PCM_SIZE;

    NoiseStreamerEncoder();
    virtual ~NoiseStreamerEncoder();

    void initForDecode();
    int decode(unsigned char* mp3Buffer, size_t mp3Len, short pcmLeft[], short pcmRight[]);
    void finilizeDecode();

    void initForEncode(EncodeContext* context);
    int encode(short int *pcmLeft, short int *pcmRight, int samples, unsigned char* mp3Buffer, size_t mp3Len);
    void finilizeEncode();
};

#endif // NoiseStreamerEncoder_h__
