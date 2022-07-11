#ifndef SimplePlaylistStrategy_h__
#define SimplePlaylistStrategy_h__

#include "PlaylistStrategy.h"

class SimplePlaylistStrategy : public PlaylistStrategy
{
public:
    SimplePlaylistStrategy(
        LogService* logSrv,
        File* playlist,
        File* history,
        bool repeat
    );
    virtual ~SimplePlaylistStrategy();

    virtual bool hasNext(PlaylistItem currentTrack);
    virtual PlaylistItem nextTrack(PlaylistItem currentTrack);
};

#endif // SimplePlaylistStrategy_h__
