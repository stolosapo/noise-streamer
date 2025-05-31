#ifndef PlaylistSource_h__
#define PlaylistSource_h__

#include <string>
#include <noisekernel/Thread.h>
#include <noisekernel/Logger.h>
#include <noisekernel/Signal.h>

#include "PlaylistAudioSourceNavigator.h"
#include "../playlist/PlaylistHandler.h"
#include "../../config/PlaylistAudioSourceConfig.h"
#include "../CircularBuffer.h"

using namespace std;
using namespace NoiseKernel;

class PlaylistSource: public PlaylistAudioSourceNavigator
{
private:
    LogService* logSrv;
    SignalAdapter* sigSrv;
    Thread* signalThread;

    Locker _playlistLock;
    PlaylistHandler* playlist;
    Thread* playingThread;

    CircularBuffer<short>* decodedBuffer;
    Thread* resampleThread;
    
    void loadPlaylist(const PlaylistAudioSourceConfig& config);
    bool hasNext();
    PlaylistItem nextTrack();

    static void* signalHandler(void* playlistSource);
    static void* startPlaying(void* playlistSource);

public:
	PlaylistSource(
        LogService* logSrv,
        SignalAdapter* sigSrv);
	virtual ~PlaylistSource();

    void initialize(const PlaylistAudioSourceConfig& config);
    void start();

    size_t readOutput(short* data, size_t num_samples);
};

#endif // PlaylistSource_h__
