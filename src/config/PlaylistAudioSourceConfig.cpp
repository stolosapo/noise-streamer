#include "PlaylistAudioSourceConfig.h"
#include "../audio/playlist/PlaylistStrategyType.h"

PlaylistAudioSourceConfig buildPlaylistAudioSourceConfig(PlaylistAudioSourceArgument* args)
{
    PlaylistAudioSourceConfig config;
    config.playlistFilePath = args->getPlaylistFilePath();
    config.historyFilePath = args->getHistoryFilePath();
    config.strategyType = convertPlaylistStrateryType(args->getStrategyType());
    config.repeat = args->getRepeat();
    return config;
}
