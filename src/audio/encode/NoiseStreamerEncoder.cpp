#include "NoiseStreamerEncoder.h"

NoiseStreamerEncoder::NoiseStreamerEncoder()
{
    lame = NULL;
}

NoiseStreamerEncoder::~NoiseStreamerEncoder()
{
    if (lame != NULL)
    {
        delete lame;
    }

}

void NoiseStreamerEncoder::initForDecode()
{
    if (lame != NULL)
    {
        delete lame;
    }

    lame = new LibLame;
    lame->init();
    lame->setDecodeOnly(true);
    lame->initParams();

#ifdef HAVE_LAME
    hip = lame->hipDecodeInit();
    memset(&mp3data, 0, sizeof(mp3data));
#endif
}

int NoiseStreamerEncoder::decode(unsigned char* mp3Buffer, size_t mp3Len, short pcmLeft[], short pcmRight[])
{
    if (lame == NULL)
    {
        return 0;
    }

#ifdef HAVE_LAME
    samples = lame->hipDecode1Headers(hip, mp3Buffer, mp3Len, pcmLeft, pcmRight, &mp3data);
    return samples;
#else
    return 0;
#endif
}

void NoiseStreamerEncoder::finilizeDecode()
{
    if (lame == NULL)
    {
        return;
    }

#ifdef HAVE_LAME
    lame->hipDecodeExit(hip);
#endif
    lame->close();
}

void NoiseStreamerEncoder::initForEncode()
{

}

void NoiseStreamerEncoder::encode()
{

}

void NoiseStreamerEncoder::finilizeEncode()
{

}
