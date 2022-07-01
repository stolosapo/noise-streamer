#ifndef PlaylistAudioSourceConfig_h__
#define PlaylistAudioSourceConfig_h__

#include <string>
#include "../audio/AudioBitrate.h"
#include "../audio/playlist/PlaylistStrategyType.h"

using namespace std;

struct PlaylistAudioSourceConfig
{
    string playlistFilePath;
    string historyFilePath;
    PlaylistStrategyType strategyType;
    bool repeat;

    bool reencode;
    AudioBitrate bitrate;
    int samplerate;
    int channels;
    string pcmOutPath;
    string mp3OutPath;
};

#endif // PlaylistAudioSourceConfig_h__
