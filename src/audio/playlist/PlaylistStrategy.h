#ifndef PlaylistStrategy_h__
#define PlaylistStrategy_h__

#include <noisekernel/Logger.h>

#include "../../utils/File.h"
#include "PlaylistStrategyType.h"
#include "PlaylistItem.h"

using namespace NoiseKernel;

class PlaylistStrategy
{
protected:
    LogService* logSrv;

    File* playlist;
    File* history;

    bool repeat;

public:
    PlaylistStrategy(
        LogService* logSrv,
        File* playlist,
        File* history,
        bool repeat
    );
    virtual ~PlaylistStrategy();

    virtual PlaylistItem getTrack(int trackIndex);
    virtual int getTrackIndex(string track);
    virtual void load();
    virtual bool hasNext(PlaylistItem currentTrack) = 0;
    virtual PlaylistItem nextTrack(PlaylistItem currentTrack) = 0;
};

#endif // PlaylistStrategy_h__
