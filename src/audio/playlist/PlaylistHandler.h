#ifndef PlaylistHandler_h__
#define PlaylistHandler_h__

#include <noisekernel/Logger.h>

#include "../../utils/File.h"

#include "PlaylistStrategyType.h"
#include "PlaylistStrategy.h"

using namespace NoiseKernel;

class PlaylistHandler
{
private:
    LogService* logSrv;

    File* playlist;
    File* history;

    PlaylistStrategy* strategy;
    PlaylistItem currentTrack;

public:
    PlaylistHandler(
        LogService* logSrv,
        string playlistFile,
        string historyFile,
        PlaylistStrategyType strategyType,
        bool repeat);
    virtual ~PlaylistHandler();

    void load();
    int playlistSize();
    int historySize();

    bool hasNext();
    PlaylistItem nextTrack();
    PlaylistItem getCurrentTrack();
    PlaylistItem getTrack(int trackIndex);
    int getTrackIndex(string track);
    string getHistoryTrack(int historyIndex);
    void archiveTrack(PlaylistItem track);
};

#endif // PlaylistHandler_h__
