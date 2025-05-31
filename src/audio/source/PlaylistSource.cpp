#include "PlaylistSource.h"

#include "../../exception/NoiseStreamerException.h"
#include "../../utils/StringHelper.h"
#include "../../utils/FileHelper.h"
#include "../decode/DecodeMP3.h"
#include "../decode/AudioDecoderFactory.h"

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
    
            // Decode file
            bool ok = self->decode(track.getTrack().c_str());
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

bool PlaylistSource::decode(const char* filename)
{
    AudioDecoder* decoder = NULL;
    bool ok;
    
    try
    {
        decoder = createDecoder(filename, sigSrv, decodedBuffer);

        ok = decoder->open(filename);

        string metadata = getMetadata(filename, decoder);
        cout << metadata << endl;

        ok = decoder->decode();
    }
    catch(DomainException& e)
    {
        logSrv->error(handle(e));
    }
    catch(RuntimeException& e)
    {
        logSrv->error(handle(e));
    }
    catch(exception& e)
    {
        logSrv->error(e.what());
    }

    if (decoder != NULL)
    {
        delete decoder;
    }

    return ok;
}

string PlaylistSource::getMetadata(const char* filepath, AudioDecoder* decoder)
{
    try
    {
        TrackMetadata metadata;
        metadata.filename = filepath;
        decoder->getMetadata(metadata);

        cout << "Metadata:" << endl;
        cout << "  Title:   " << metadata.title << endl;
        cout << "  Artist:  " << metadata.artist << endl;
        cout << "  Album:   " << metadata.album << endl;
        cout << "  Year:    " << metadata.year << endl;
        cout << "  Comment: " << metadata.comment << endl;
        cout << "  Genre:   " << metadata.genre << endl;
        cout << "  Duration:   " << metadata.duration << endl;
        cout << "  Bitrate:   " << metadata.bitrate << endl;
        cout << "  Samplerate:   " << metadata.samplerate << endl;
        cout << "  Channels:   " << metadata.channels << endl;

        return metadata.artist + " - " + metadata.title;
    }
    catch(DomainException& e)
    {
        logSrv->warn(handle(e));
    }
    catch(RuntimeException& e)
    {
        logSrv->warn(handle(e));
    }
    catch(exception& e)
    {
        logSrv->warn(e.what());
    }

    return filename(filepath);
}