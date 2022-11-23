#ifndef PlaylistAudioSource_h__
#define PlaylistAudioSource_h__

#include <string>
#include <noisekernel/Logger.h>
#include <noisekernel/Signal.h>
#include <noisekernel/Thread.h>

#include "AudioSource.h"
#include "PlaylistAudioSourceNavigator.h"
#include "PlaylistAudioSourceItem.h"
#include "PlaylistAudioSourceTask.h"
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
    NoiseStreamerEncoder* mp3Decoder;

    unsigned char *mp3Buffer;
    int numberOfPlayedTracks;
    int decodeErrorCnt;
    FILE *currentMp3File;
    PlaylistItem currentTrack;
    PlaylistAudioSourceItem* currentPlaylistItem;
    time_t currentTrackStartTime;
    TaskRunner* taskRunner;

    bool loadNextPlaylistItem();
    void prepareNextTrack();
    void finilizeCurrentPlayingTrack();

    friend void* playlistaudiosource_audio_status(void* task);
    friend void* playlistaudiosource_now_playing(void* task);
    friend void* playlistaudiosource_preview_next(void* task);
    friend void* playlistaudiosource_preview_track(void* task);
    friend void* playlistaudiosource_search(void* task);
    friend void* playlistaudiosource_history(void* task);
    friend void* playlistaudiosource_next_track(void* task);
    friend void* playlistaudiosource_request_track(void* task);

public:
	PlaylistAudioSource(
        LogService* logSrv,
        SignalAdapter* sigSrv,
        PlaylistAudioSourceConfig* config);
	virtual ~PlaylistAudioSource();

    virtual void* runCommand(string command);
    virtual void shutdownAudioSource();
    virtual void initialize();
    virtual int readNextMp3Data(unsigned char* mp3OutBuffer, size_t buffer_size);
    virtual int readNextPcmData(short *pcmLeft, short *pcmRight);
};

#endif // PlaylistAudioSource_h__
