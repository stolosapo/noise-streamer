#ifndef EncodeContext_h__
#define EncodeContext_h__

#include "../AudioEncodeMode.h"

struct EncodeContext
{
    AudioEncodeMode encodeMode;
    int bitrate;
    int samplerate;
    int channels;
    int quality;
};

#endif // EncodeContext_h__
