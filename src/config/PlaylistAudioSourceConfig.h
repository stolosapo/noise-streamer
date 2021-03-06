#ifndef PlaylistAudioSourceConfig_h__
#define PlaylistAudioSourceConfig_h__

#include <string>
#include "../audio/AudioBitrate.h"
#include "../audio/playlist/PlaylistStrategyType.h"
#include "../argument/PlaylistAudioSourceArgument.h"

using namespace std;

struct PlaylistAudioSourceConfig
{
    string playlistFilePath;
    string historyFilePath;
    PlaylistStrategyType strategyType;
    bool repeat;
};

PlaylistAudioSourceConfig buildPlaylistAudioSourceConfig(PlaylistAudioSourceArgument* args);

#endif // PlaylistAudioSourceConfig_h__
