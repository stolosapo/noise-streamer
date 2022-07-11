#include "PlaylistStrategy.h"
#include "../tag/AudioTagReader.h"

PlaylistStrategy::PlaylistStrategy(
    LogService* logSrv,
    File* playlist,
    File* history,
    bool repeat)
    : logSrv(logSrv),
    playlist(playlist),
    history(history),
    repeat(repeat)
{

}

PlaylistStrategy::~PlaylistStrategy()
{

}

PlaylistItem PlaylistStrategy::getTrack(int trackIndex)
{
    string track = playlist->read(trackIndex);
    AudioTag* meta = readAudioTag(track.c_str());

    return PlaylistItem(trackIndex, track, meta);
}

int PlaylistStrategy::getTrackIndex(string track)
{
    return playlist->read(track);
}

void PlaylistStrategy::load()
{

}
