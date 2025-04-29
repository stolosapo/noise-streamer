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
    // decodedBuffer = new CircularBuffer(sigSrv, 44100 * 2); // 1 sec buffer (44.1kHz stereo)
    decodedBuffer = new CircularBuffer(sigSrv, 44100 * 2 * 10); // 10 sec buffer (44.1kHz stereo)
    decodedBuffer_v2 = new CircularBuffer_v2<short>(sigSrv, 44100 * 2 * 2);
    // pcmOutputBuffer = new CircularBuffer(sigSrv, 44100 * 2); // 1 sec buffer (44.1kHz stereo)
    pcmOutputBuffer = new CircularBuffer(sigSrv, 44100 * 2 * 10); // 10 sec buffer (44.1kHz stereo)
    pcmOutputBuffer_v2 = new CircularBuffer_v2<short>(sigSrv, 44100 * 2 * 2);

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

    if (decodedBuffer_v2 != NULL)
    {
        delete decodedBuffer_v2;
    }

    if (pcmOutputBuffer != NULL)
    {
        delete pcmOutputBuffer;
    }

    if (pcmOutputBuffer_v2 != NULL)
    {
        delete pcmOutputBuffer_v2;
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
    self->decodedBuffer_v2->close();
    self->pcmOutputBuffer->reset();
    self->pcmOutputBuffer_v2->close();

    return NULL;
}

size_t PlaylistSource::readOutput(short* data, size_t num_samples)
{
    // pcmOutputBuffer->read(data, num_samples);
    // return num_samples;
    return decodedBuffer_v2->read(data, num_samples);
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

        // decode_mp3(track.getTrack().c_str(), self->pcmOutputBuffer, self->sigSrv, rate, channels, encoding);
        bool ok = decode_mp3(track.getTrack().c_str(), self->decodedBuffer_v2, self->sigSrv, rate, channels, encoding);
        if (!ok)
        {
            cerr << "Decode error! Skipping Track..." << endl;
            continue;
        }

        self->currentTrackRate = rate;
        self->currentTrackChannels = channels;
        self->currentTrackEncoding = encoding;

        // Archive it..
        self->playlist->archiveTrack(track);
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
        resample_pcm(self->decodedBuffer_v2, self->pcmOutputBuffer_v2, self->currentTrackRate, newSamplerate);
    }

    self->logSrv->info("Resample process ended");

    return NULL;
}