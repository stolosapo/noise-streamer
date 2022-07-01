#include "AudioSource.h"

AudioSource::AudioSource(const AudioSourceType type): type(type)
{
    ErrorAppeared = Event();
    AudioMetadataChanged = Event();
}

AudioSource::~AudioSource()
{
}

AudioSourceType AudioSource::getType() const
{
    return type;
}
