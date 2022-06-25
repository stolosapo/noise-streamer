#ifndef AudioSource_h__
#define AudioSource_h__

#include <string>

#include "AudioSourceType.h"

using namespace std;

class AudioSource
{
private:
    const AudioSourceType type;

public:
    AudioSource(const AudioSourceType type);
    virtual ~AudioSource();

    AudioSourceType getType() const;

    virtual void shutdownAudioSource() = 0;
    virtual void initialize() = 0;
    virtual int readNextMp3Data(unsigned char* mp3OutBuffer, size_t buffer_size) = 0;
};

#endif // AudioSource_h__
