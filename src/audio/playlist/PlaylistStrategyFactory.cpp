#include "PlaylistStrategyFactory.h"
#include "SimplePlaylistStrategy.h"
#include "RandomOncePlaylistStrategy.h"

PlaylistStrategy* createPlaylistStrategy(
    PlaylistStrategyType type,
    LogService* logSrv,
    File* playlist,
    File* history,
    bool repeat)
{
    switch (type)
    {
    case NONE:
        return new SimplePlaylistStrategy(logSrv, playlist, history, repeat);

    case SIMPLE:
        return new SimplePlaylistStrategy(logSrv, playlist, history, repeat);

    case RANDOM_ONCE:
        return new RandomOncePlaylistStrategy(logSrv, playlist, history, repeat);

    default:
        return new SimplePlaylistStrategy(logSrv, playlist, history, repeat);
    }
}
