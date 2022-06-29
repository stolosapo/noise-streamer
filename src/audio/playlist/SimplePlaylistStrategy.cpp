#include "SimplePlaylistStrategy.h"

SimplePlaylistStrategy::SimplePlaylistStrategy(
    LogService* logSrv,
    File* playlist,
    File* history,
    bool repeat)
    : PlaylistStrategy(logSrv, playlist, history, repeat)
{

}

SimplePlaylistStrategy::~SimplePlaylistStrategy()
{

}

bool SimplePlaylistStrategy::hasNext(PlaylistItem currentTrack)
{
    if (repeat)
    {
        return true;
    }

    int index = currentTrack.getTrackIndex();
    int size = playlist->size();

    return index < size - 1;
}

PlaylistItem SimplePlaylistStrategy::nextTrack(PlaylistItem currentTrack)
{
    int currentTrackIndex = currentTrack.getTrackIndex();

    currentTrackIndex++;

    if (currentTrackIndex >= playlist->size())
    {
        currentTrackIndex = 0;
    }

    return getTrack(currentTrackIndex);
}
