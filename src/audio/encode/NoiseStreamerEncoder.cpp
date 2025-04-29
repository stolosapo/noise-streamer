#include "NoiseStreamerEncoder.h"

#include <stdio.h>
#include <cstring>
#include "../../utils/StringHelper.h"

const int NoiseStreamerEncoder::MP3_SIZE = 4096;
const int NoiseStreamerEncoder::PCM_SIZE = MP3_SIZE * 100;

NoiseStreamerEncoder::NoiseStreamerEncoder()
{
    lame = NULL;
}

NoiseStreamerEncoder::~NoiseStreamerEncoder()
{
    if (lame != NULL)
    {
        delete lame;
        lame = NULL;
    }
}

void NoiseStreamerEncoder::setId3Tag(AudioTag* settings)
{
    lame->id3tagInit();
    lame->id3tagV2Only();

    if (settings->getYear() != 0)
    {
        lame->id3tagSetYear(numberToString<int>(settings->getYear()));
    }

    lame->id3tagSetGenre(settings->getGenre());
    lame->id3tagSetArtist(settings->getArtist());
    lame->id3tagSetAlbum(settings->getAlbum());
    lame->id3tagSetTitle(settings->getTitle());
    lame->id3tagSetTrack(numberToString<int>(settings->getTrack()));
    lame->id3tagSetComment(settings->getComments());
    // lame->id3tagSetAlbumArt(...);

    lame->setWriteId3tagAutomatic(false);
}

void NoiseStreamerEncoder::setChannels(int channels)
{
    if (channels == 1)
    {
        lame->setModeMono();
        lame->setNumChannels(1);
    }
    else
    {
        lame->setModeJointStereo();
        lame->setNumChannels(2);
    }
}

void NoiseStreamerEncoder::setVbrTag(EncodeContext *context)
{
    lame->setBWriteVbrTag(true);

    switch (context->encodeMode)
    {
        case ABR:
            setEncodeModeABR(context->bitrate);
            break;
        case CBR:
            setEncodeModeCBR(context->quality, context->bitrate);
            break;
        case VBR:
            setEncodeModeVBR(context->quality);
            break;
        default:
            break;
    }
}

void NoiseStreamerEncoder::setEncodeModeABR(int bitrate)
{
    if(bitrate >= 8000)
    {
        bitrate = (bitrate + 500) / 1000;
    }

    if(bitrate > 320)
    {
        bitrate = 320;
    }

    if(bitrate < 8)
    {
        bitrate = 8;
    }

    lame->setVBR_ABR();
    lame->setVBRMeanBitrateKbps(bitrate);
}

void NoiseStreamerEncoder::setEncodeModeCBR(int quality, int bitrate)
{
    lame->setVBR_OFF();
    lame->setQuality(quality);
    lame->setBrate(bitrate);
}

void NoiseStreamerEncoder::setEncodeModeVBR(int quality)
{
    lame->setVBR_MTRH();
    lame->setVBRq(quality);
}

void NoiseStreamerEncoder::initForDecode()
{
    if (lame != NULL)
    {
        finilizeDecode();
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
        return -1;
    }

#ifdef HAVE_LAME
    return lame->hipDecodeHeaders(hip, mp3Buffer, mp3Len, pcmLeft, pcmRight, &mp3data);
#else
    return -1;
#endif
}

int NoiseStreamerEncoder::getDecodedHeaderParsed()
{
#ifdef HAVE_LAME
    // 1 if header was parsed and following data was computed
    return mp3data.header_parsed;
#else
    return -1;
#endif
}

int NoiseStreamerEncoder::getDecodedStereo()
{
#ifdef HAVE_LAME
    // number of channels
    return mp3data.stereo;
#else
    return -1;
#endif
}

int NoiseStreamerEncoder::getDecodedSamplerate()
{
#ifdef HAVE_LAME
    return mp3data.samplerate;
#else
    return -1;
#endif
}

int NoiseStreamerEncoder::getDecodedBitrate()
{
#ifdef HAVE_LAME
    return mp3data.bitrate;
#else
    return -1;
#endif
}

int NoiseStreamerEncoder::getDecodedMode()
{
#ifdef HAVE_LAME
    // mp3 frame type
    return mp3data.mode;
#else
    return -1;
#endif
}

int NoiseStreamerEncoder::getDecodedModeExt()
{
#ifdef HAVE_LAME
    // mp3 frame type
    return mp3data.mode_ext;
#else
    return -1;
#endif
}

int NoiseStreamerEncoder::getDecodedFramesize()
{
#ifdef HAVE_LAME
    // number of samples per mp3 frame
    return mp3data.framesize;
#else
    return -1;
#endif
}

unsigned long NoiseStreamerEncoder::getDecodedNumberOfSamples()
{
#ifdef HAVE_LAME
    // this data is only computed if mpglib detects a Xing VBR header
    // number of samples in mp3 file.
    return mp3data.nsamp;
#else
    return -1;
#endif
}

int NoiseStreamerEncoder::getDecodedTotalFrames()
{
#ifdef HAVE_LAME
    // total number of frames in mp3 file
    return mp3data.totalframes;
#else
    return -1;
#endif
}

int NoiseStreamerEncoder::getDecodedFrameNumber()
{
#ifdef HAVE_LAME
    // this data is not currently computed by the mpglib routines
    // frames decoded counter
    return mp3data.framenum;
#else
    return -1;
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

void NoiseStreamerEncoder::initForEncode(EncodeContext* context)
{
    if (lame != NULL)
    {
        delete lame;
    }

    lame = new LibLame;
    lame->init();

    // lame->setInSamplerate(settings->getSamplerate());
    lame->setOutSamplerate(context->samplerate);
    lame->setFindReplayGain(true);

    // setId3Tag(settings);
    setChannels(context->channels);
    setVbrTag(context);

    lame->initParams();
}

int NoiseStreamerEncoder::encode(short* pcmLeft, short* pcmRight, int samples, unsigned char* mp3Buffer, size_t mp3Len)
{
    return lame->encodeBuffer(pcmLeft, pcmRight, samples, mp3Buffer, mp3Len);
}

int NoiseStreamerEncoder::encode(short* pcm_buffer, int num_samples, unsigned char* mp3_buffer, int mp3_buffer_size)
{
    return lame->encodeBufferInterleaved(pcm_buffer, num_samples, mp3_buffer, mp3_buffer_size);
}

void NoiseStreamerEncoder::finilizeEncode()
{
    if (lame == NULL)
    {
        return;
    }
    lame->close();
}
