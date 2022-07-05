#include "PlaylistAudioSourceArgument.h"
#include "../exception/NoiseStreamerException.h"

const string PlaylistAudioSourceArgument::PLAYLISTFILEPATH = "playlistfile";
const string PlaylistAudioSourceArgument::HISTORYFILEPATH = "historyfile";
const string PlaylistAudioSourceArgument::STRATEGYTYPE = "strategy";
const string PlaylistAudioSourceArgument::REPEAT = "repeat";
const string PlaylistAudioSourceArgument::REENCODE = "reencode";
const string PlaylistAudioSourceArgument::PCMOUTPATH = "pcmoutpath";
const string PlaylistAudioSourceArgument::MP3OUTPATH = "mp3outpath";

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
    registerArg(REENCODE, "If should reencode tracks");
    registerArg(PCMOUTPATH, "The full path for decode mp3 to pcm files");
    registerArg(MP3OUTPATH, "The full path for reencoded mp3 file");
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

bool PlaylistAudioSourceArgument::getReencode()
{
    return getBoolValue(REENCODE);
}

string PlaylistAudioSourceArgument::getPcmOutPath()
{
    return getStringValue(PCMOUTPATH);
}

string PlaylistAudioSourceArgument::getMp3OutPath()
{
    return getStringValue(MP3OUTPATH);
}

