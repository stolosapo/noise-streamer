#include "LibShout.h"

void LibShout::setAudioInfo(string name, string value)
{
    if (shout_set_audio_info(shout, name.c_str(), value.c_str()) != SHOUTERR_SUCCESS)
    {
        string mess = name + ":" + value;
        // throw DomainException(NoiseStreamerDomainErrorCode::NSS0014, getError());
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
