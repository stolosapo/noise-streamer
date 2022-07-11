#ifndef PlaylistAudioSource_h__
#define PlaylistAudioSource_h__

#include <string>
#include <noisekernel/Logger.h>
#include <noisekernel/Signal.h>
#include <noisekernel/Thread.h>

#include "AudioSource.h"
#include "PlaylistAudioSourceNavigator.h"
#include "PlaylistAudioSourceItem.h"
#include "PlaylistAudioSourceItemContext.h"
#include "../playlist/PlaylistHandler.h"
#include "../encode/NoiseStreamerEncoder.h"
#include "../../config/PlaylistAudioSourceConfig.h"
#include "../../utils/SynchronizedQueue.h"
#include "../../utils/SynchronizedPQueue.h"

using namespace std;
using namespace NoiseKernel;

class PlaylistAudioSource: public AudioSource, public PlaylistAudioSourceNavigator
{
private:
    LogService* logSrv;
    SignalAdapter* sigSrv;
    PlaylistAudioSourceConfig* config;

    PlaylistHandler* playlistHandler;
    SynchronizedQueue<int> requestedTrackIndex;
    SynchronizedPQueue<PlaylistAudioSourceItem> mainQueue;
    ThreadPool* encodePool;
    NoiseStreamerEncoder* mp3Decoder;

    unsigned char *mp3Buffer;
    int numberOfPlayedTracks;
    int decodeErrorCnt;
    FILE *currentMp3File;
    PlaylistItem currentTrack;
    PlaylistAudioSourceItem* currentPlaylistItem;
    time_t currentTrackStartTime;

    PlaylistAudioSourceItem* createPlaylistAudioSourceItem(PlaylistItem item);
    PlaylistAudioSourceItem* fetchNextPlaylistItem();
    bool loadNextPlaylistItem();
    void prepareNextTrack();
    void finilizeCurrentPlayingTrack();
    void archiveTrack(PlaylistAudioSourceItem* item);

public:
	PlaylistAudioSource(
        LogService* logSrv,
        SignalAdapter* sigSrv,
        PlaylistAudioSourceConfig* config);
	virtual ~PlaylistAudioSource();

    virtual void shutdownAudioSource();
    virtual void initialize();
    virtual int readNextMp3Data(unsigned char* mp3OutBuffer, size_t buffer_size);
    virtual int readNextPcmData(short *pcmLeft, short *pcmRight);
};

#endif // PlaylistAudioSource_h__
