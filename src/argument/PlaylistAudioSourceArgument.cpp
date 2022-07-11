#include "PlaylistAudioSourceArgument.h"
#include "../exception/NoiseStreamerException.h"

const string PlaylistAudioSourceArgument::PLAYLISTFILEPATH = "playlistfile";
const string PlaylistAudioSourceArgument::HISTORYFILEPATH = "historyfile";
const string PlaylistAudioSourceArgument::STRATEGYTYPE = "strategy";
const string PlaylistAudioSourceArgument::REPEAT = "repeat";

PlaylistAudioSourceArgument::PlaylistAudioSourceArgument(
    ArgumentProvider* argProvider): ArgumentAdapter(argProvider)
{

}

PlaylistAudioSourceArgument::~PlaylistAudioSourceArgument()
{

}

string PlaylistAudioSourceArgument::title()
{
    return "Playlist AudioSource help";
}

void PlaylistAudioSourceArgument::registerArguments()
{
    registerArg(PLAYLISTFILEPATH, "The full filename path for playlist");
    registerArg(HISTORYFILEPATH, "The full filename path for history");
    registerArg(STRATEGYTYPE, "The playlist strategy: SIMPLE, RANDOM_ONCE");
    registerArg(REPEAT, "If playlist should repear after finish: true, false");
}

string PlaylistAudioSourceArgument::getPlaylistFilePath()
{
    string s = getStringValue(PLAYLISTFILEPATH);
    if (s == "")
    {
        throw DomainException(ARG0001, PLAYLISTFILEPATH);
    }
    return s;
}

string PlaylistAudioSourceArgument::getHistoryFilePath()
{
    string s = getStringValue(HISTORYFILEPATH);
    if (s == "")
    {
        throw DomainException(ARG0001, HISTORYFILEPATH);
    }
    return s;
}

string PlaylistAudioSourceArgument::getStrategyType()
{
    return getStringValue(STRATEGYTYPE);
}

bool PlaylistAudioSourceArgument::getRepeat()
{
    return getBoolValue(REPEAT);
}
