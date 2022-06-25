#include "LibShout.h"
#include <noisekernel/Exception.h>
#include "../exception/NoiseStreamerException.h"

using namespace NoiseKernel;

void LibShout::setAudioInfo(string name, string value)
{
    if (shout_set_audio_info(shout, name.c_str(), value.c_str()) != SHOUTERR_SUCCESS)
    {
        string mess = name + ":" + value;
        throw DomainException(NSS0013, getError());
    }
}

void LibShout::setAudioInfoBitrate(string value)
{
    setAudioInfo(SHOUT_AI_BITRATE, value);
}

void LibShout::setAudioInfoSamplerate(string value)
{
    setAudioInfo(SHOUT_AI_SAMPLERATE, value);
}

void LibShout::setAudioInfoChannels(string value)
{
    setAudioInfo(SHOUT_AI_CHANNELS, value);
}

void LibShout::setAudioInfoQuality(string value)
{
    setAudioInfo(SHOUT_AI_QUALITY, value);
}
