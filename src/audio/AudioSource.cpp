#include "AudioSource.h"

AudioSource::AudioSource(const AudioSourceType type): type(type)
{
}

AudioSource::~AudioSource()
{
}

AudioSourceType AudioSource::getType() const
{
    return type;
}
