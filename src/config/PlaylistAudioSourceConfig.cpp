#include "PlaylistAudioSourceConfig.h"
#include <vector>
#include <noisekernel/Exception.h>
#include "../audio/playlist/PlaylistStrategyType.h"
#include "../utils/File.h"
#include "../utils/StringHelper.h"

using namespace std;
using namespace NoiseKernel;

PlaylistAudioSourceConfig buildPlaylistAudioSourceConfig(PlaylistAudioSourceArgument* args)
{
    PlaylistAudioSourceConfig config;
    config.playlistFilePath = args->getPlaylistFilePath();
    config.historyFilePath = args->getHistoryFilePath();
    config.strategyType = convertPlaylistStrateryType(args->getStrategyType());
    config.repeat = args->getRepeat();
    return config;
}

void buildPlaylistAudioSourceConfig(const string& configFilePath, PlaylistAudioSourceConfig& config)
{
    if (configFilePath == "")
    {
        throw RuntimeException("Could not find config file");
    }

    File configFile(configFilePath);
    configFile.load();

    config.playlistFilePath = "";
    config.historyFilePath = "";
    config.strategyType = NONE;
    config.repeat = false;

    for (size_t i = 0; i < configFile.size(); i++)
    {
        string line = configFile.read(i);
        vector<string> values = split(line, "=");

        if (values.size() <= 1)
        {
            continue;
        }

        string param = values[0];
        string paramValue = values[1];

        if (param == "playlistfile")
        {
            config.playlistFilePath = paramValue;
        } 
        else if (param == "historyfile")
        {
            config.historyFilePath = paramValue;
        } 
        else if (param == "strategy")
        {
            config.strategyType = convertPlaylistStrateryType(paramValue);
        }
        else if (param == "repeat" && paramValue == "true")
        {
            config.repeat = true;
        }
    }
}