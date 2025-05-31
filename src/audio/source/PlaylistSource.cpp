#include "PlaylistSource.h"

#include "../../utils/StringHelper.h"
#include "../decode/DecodeMP3.h"

#include <noisekernel/Exception.h>

using namespace NoiseKernel;

PlaylistSource::PlaylistSource(
    LogService* logSrv,
    SignalAdapter* sigSrv): 
    PlaylistAudioSourceNavigator(),
    logSrv(logSrv), 
    sigSrv(sigSrv),
    signalThread(NULL),
    playlist(NULL),
    playingThread(NULL)
{
    decodedBuffer = new CircularBuffer<short>(44100 * 2 * 2);
    _playlistLock.init();
}

PlaylistSource::~PlaylistSource()
{
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

    _playlistLock.destroy();
}

void* PlaylistSource::signalHandler(void* playlistSource)
{
    PlaylistSource* self = (PlaylistSource*) playlistSource;

    while(!self->sigSrv->gotSigInt())
    {
        usleep(500);
    }

    self->decodedBuffer->close();
    
    return NULL;
}

size_t PlaylistSource::readOutput(short* data, size_t num_samples)
{
    return decodedBuffer->read(data, num_samples);
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

    logSrv->info("Playlist initialized!");
    logSrv->trace("Playlist start loading..");

    playlist->load();

    _playlistLock.unlock();

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
    if (playingThread != NULL)
    {
        logSrv->warn("Playing thread already is running, skipping..");
        return;
    }
    
    // Start Decode and Playing thread
    playingThread = new Thread();
    playingThread->attachDelegate(&PlaylistSource::startPlaying);
    playingThread->start(this);

    if (signalThread != NULL)
    {
        logSrv->warn("Signal thread already is running, skipping..");
        return;
    }

    // Start Signal Thread
    signalThread = new Thread();
    signalThread->attachDelegate(&PlaylistSource::signalHandler);
    signalThread->start(this);
}

void* PlaylistSource::startPlaying(void* playlistSource)
{
    PlaylistSource* self = (PlaylistSource*) playlistSource;

    try
    {
        // Run playlist is ended
        while (!self->sigSrv->gotSigInt() && self->hasNext() && !self->isStoped())
        {
            // Get next track
            PlaylistItem track = self->nextTrack();
            self->logSrv->info("Playing: " + track.getTrack());
    
            // TODO: Raise AudioMetadataChanged Event
            
            // Decode file
            long rate;
            int channels;
            int encoding;
    
            bool ok = decode_mp3(track.getTrack().c_str(), self->decodedBuffer, self->sigSrv, rate, channels, encoding);
            if (!ok)
            {
                // TODO: Considering put this track to failed list
                self->logSrv->warn("Decode error! Skipping Track...");
                continue;
            }
    
            // Archive it..
            self->playlist->archiveTrack(track);
        }
    
        self->logSrv->info("PlaylistSource stopped playing");
    }
    catch(DomainException& e)
    {
        self->logSrv->error(handle(e));
    }
    catch(RuntimeException& e)
    {
        self->logSrv->error(handle(e));
    }
    catch(exception& e)
    {
        self->logSrv->error(e.what());
    }
    
    return NULL;
}
