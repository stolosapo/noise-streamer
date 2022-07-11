#include "PlaylistHandler.h"

#include "../../exception/NoiseStreamerException.h"
#include "PlaylistStrategyFactory.h"

PlaylistHandler::PlaylistHandler(
    LogService* logSrv,
    string playlistFile,
    string historyFile,
    PlaylistStrategyType strategyType,
    bool repeat): logSrv(logSrv)
{
    playlist = new File(playlistFile);
    history = new File(historyFile);
    strategy = createPlaylistStrategy(strategyType, logSrv, playlist, history, repeat);
}

PlaylistHandler::~PlaylistHandler()
{
    delete playlist;
    delete history;
    delete strategy;
}

PlaylistItem PlaylistHandler::getCurrentTrack()
{
    return currentTrack;
}

PlaylistItem PlaylistHandler::getTrack(int trackIndex)
{
    return strategy->getTrack(trackIndex);
}

int PlaylistHandler::getTrackIndex(string track)
{
    return strategy->getTrackIndex(track);
}

string PlaylistHandler::getHistoryTrack(int historyIndex)
{
    return history->read(historyIndex);
}

void PlaylistHandler::load()
{
    playlist->load();
    history->loadRestOf(playlist->size());
    strategy->load();
}

int PlaylistHandler::playlistSize()
{
    return playlist->size();
}

int PlaylistHandler::historySize()
{
    return history->size();
}

bool PlaylistHandler::hasNext()
{
    return strategy->hasNext(currentTrack);
}

PlaylistItem PlaylistHandler::nextTrack()
{
    currentTrack = strategy->nextTrack(currentTrack);
    return currentTrack;
}

void PlaylistHandler::archiveTrack(PlaylistItem track)
{
    history->append(track.getTrack());
}
