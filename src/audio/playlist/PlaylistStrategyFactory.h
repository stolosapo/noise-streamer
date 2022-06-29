#ifndef PlaylistStrategyFactory_h__
#define PlaylistStrategyFactory_h__

#include <noisekernel/Logger.h>

#include "../../utils/File.h"
#include "PlaylistStrategy.h"

using namespace NoiseKernel;

PlaylistStrategy* createPlaylistStrategy(
    PlaylistStrategyType type,
    LogService* logSrv,
    File* playlist,
    File* history,
    bool repeat);

#endif // PlaylistStrategyFactory_h__
