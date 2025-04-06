#include "PlaylistSource.h"

#include "../../utils/StringHelper.h"
#include "../decode/DecodeMP3.h"
#include "../resample/resample.h"
#include "../BufferSizes.h"

PlaylistSource::PlaylistSource(
    LogService* logSrv,
    SignalAdapter* sigSrv): 
    PlaylistAudioSourceNavigator(),
    logSrv(logSrv), 
    sigSrv(sigSrv),
    signalThread(NULL),
    playlist(NULL),
    playingThread(NULL),
    resampleThread(NULL)
{
    decodedBuffer = new CircularBuffer(sigSrv, PCM_BUFFER_SAMPLES); // 1 sec buffer (44.1kHz stereo)
    // decodedBuffer = new CircularBuffer(sigSrv, 44100 * 2); // 1 sec buffer (44.1kHz stereo)
    pcmOutputBuffer = new CircularBuffer(sigSrv, PCM_BUFFER_SAMPLES); // 1 sec buffer (44.1kHz stereo)
    // pcmOutputBuffer = new CircularBuffer(sigSrv, 44100 * 2); // 1 sec buffer (44.1kHz stereo)

    _playlistLock.init();
}

PlaylistSource::~PlaylistSource()
{
    if (resampleThread != NULL)
    {
        resampleThread->wait();
        delete resampleThread;
    }

    if (playingThread != NULL)
    {
        playingThread->wait();
        delete playingThread;
    }

    if (signalThread != NULL)
    {
        signalThread->wait();
        delete signalThread;
    }

    if (playlist != NULL)
    {
        delete playlist;
    }

    if (decodedBuffer != NULL)
    {
        delete decodedBuffer;
    }

    if (pcmOutputBuffer != NULL)
    {
        delete pcmOutputBuffer;
    }

    _playlistLock.destroy();
}

void* PlaylistSource::signalHandler(void* playlistSource)
{
    PlaylistSource* self = (PlaylistSource*) playlistSource;

    while(!self->sigSrv->gotSigInt())
    {
        usleep(500);
    }

    self->decodedBuffer->reset();
    self->pcmOutputBuffer->reset();

    return NULL;
}

void PlaylistSource::readOutput(short* data, size_t num_samples)
{
    pcmOutputBuffer->read(data, num_samples);
}

void PlaylistSource::loadPlaylist(const PlaylistAudioSourceConfig& config)
{
    _playlistLock.lock();

    playlist = new PlaylistHandler(
        logSrv,
        config.playlistFilePath,
        config.historyFilePath,
        config.strategyType,
        config.repeat);

    _playlistLock.unlock();

    logSrv->info("Playlist initialized!");
    logSrv->trace("Playlist start loading..");

    playlist->load();

    logSrv->debug("Playlist: '" + config.playlistFilePath + "' loaded, with '" + numberToString<int>(playlist->playlistSize()) + "' tracks");
    logSrv->debug("History: '" + config.historyFilePath + "' loaded, with '" + numberToString<int>(playlist->historySize()) + "' tracks");
}

bool PlaylistSource::hasNext()
{
    _playlistLock.lock();
    bool has = playlist->hasNext();
    _playlistLock.unlock();
    
    return has;
}

PlaylistItem PlaylistSource::nextTrack()
{
    _playlistLock.lock();
    PlaylistItem next = playlist->nextTrack();
    _playlistLock.unlock();

    return next;
}

void PlaylistSource::initialize(const PlaylistAudioSourceConfig& config)
{
    loadPlaylist(config);
}

void PlaylistSource::start()
{
    // Start Decode and Playing thread
    playingThread = new Thread();
    playingThread->attachDelegate(&PlaylistSource::startPlaying);
    playingThread->start(this);

    // Start Resample Thread
    // resampleThread = new Thread();
    // resampleThread->attachDelegate(&PlaylistSource::resample);
    // resampleThread->start(this);

    // Start Signal Thread
    signalThread = new Thread();
    signalThread->attachDelegate(&PlaylistSource::signalHandler);
    signalThread->start(this);
}

void* PlaylistSource::startPlaying(void* playlistSource)
{
    PlaylistSource* self = (PlaylistSource*) playlistSource;

    // Run playlist is ended
    while (!self->sigSrv->gotSigInt() && self->hasNext())
    {
        // Get next track
        PlaylistItem track = self->nextTrack();
        self->logSrv->info("Playing: " + track.getTrack());

        // TODO: Raise AudioMetadataChanged Event
        
        // Decode file
        long rate;
        int channels;
        int encoding;

        decode_mp3(track.getTrack().c_str(), self->pcmOutputBuffer, self->sigSrv, rate, channels, encoding);

        self->currentTrackRate = rate;
        self->currentTrackChannels = channels;
        self->currentTrackEncoding = encoding;
    }

    self->logSrv->info("PlaylistSource stopped playing");

    return NULL;
}

void* PlaylistSource::resample(void* playlistSource)
{
    PlaylistSource* self = (PlaylistSource*) playlistSource;

    const size_t buffer_size = RESAMPLE_CHUNK; // Adjust as needed
    short decoded_buffer[buffer_size];

    int originalSamplerate = 44100;
    int newSamplerate = 44100;

    while (!self->sigSrv->gotSigInt()) 
    {
        resample_pcm(self->decodedBuffer, self->pcmOutputBuffer, self->currentTrackRate, newSamplerate);
    }

    self->logSrv->info("Resample process ended");

    return NULL;
}