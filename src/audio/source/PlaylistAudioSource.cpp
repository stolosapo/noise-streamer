#include "PlaylistAudioSource.h"
#include <time.h>
#include <iostream>
#include <noisekernel/Exception.h>
#include "AudioSourceType.h"
#include "AudioMetadataChangedEventArgs.h"
#include "../playlist/PlaylistHandler.h"
#include "../../utils/StringHelper.h"
#include "../../utils/FileHelper.h"

using namespace std;
using namespace NoiseKernel;

PlaylistAudioSource::PlaylistAudioSource(
    LogService* logSrv,
    SignalAdapter* sigSrv,
    PlaylistAudioSourceConfig* config)
    : AudioSource(SOURCE_PLAYLIST),
    PlaylistAudioSourceNavigator(),
    logSrv(logSrv),
    sigSrv(sigSrv),
    config(config)
{
    playlistHandler = NULL;
    numberOfPlayedTracks = 0;
    currentMp3File = NULL;
    currentPlaylistItem = NULL;
    currentTrackStartTime = 0;
    encodePool = new ThreadPool(5);
}

PlaylistAudioSource::~PlaylistAudioSource()
{
    if (playlistHandler != NULL)
    {
        delete playlistHandler;
    }

    if (currentPlaylistItem != NULL)
    {
        delete currentPlaylistItem;
        currentPlaylistItem = NULL;
    }

    finilizeCurrentPlayingTrack();

    delete encodePool;
}

bool PlaylistAudioSource::needReEncode(PlaylistItem& item)
{
    if (!config->reencode)
    {
        return false;
    }

    AudioTag* metadata = item.getMetadata();
    int itemSamplerate = metadata->getSamplerate();
    int itemChannels = metadata->getChannels();

    int confSamplerate = config->samplerate;
    int confChannels = config->channels;

    return (itemSamplerate != confSamplerate) || (itemChannels != confChannels);
}

PlaylistAudioSourceItem* PlaylistAudioSource::createPlaylistAudioSourceItem(PlaylistItem item)
{
    PlaylistAudioSourceItem* playlistItem = NULL;

    if (!needReEncode(item))
    {
        playlistItem = new PlaylistAudioSourceItem(item);
    }
    else if (encodePool->hasNext())
    {
        PlaylistAudioSourceItemContext context;
        context.encodeThread = encodePool->getNext();
        context.logSrv = logSrv;
        context.pcmOutPath = config->pcmOutPath;
        context.mp3OutPath = config->mp3OutPath;
        context.encodeMode = VBR;
        context.audioBitrate = config->bitrate;
        context.samplerate = config->samplerate;
        context.quality = 3;

        playlistItem = new PlaylistAudioSourceItem(item, context);
    }
    else
    {
        logSrv->warn("Encode Pool has no more free threads, so encoding of track will be bypassed");
        playlistItem = new PlaylistAudioSourceItem(item);
    }

    if (playlistItem != NULL)
    {
        playlistItem->prepare();
    }

    return playlistItem;
}

PlaylistAudioSourceItem* PlaylistAudioSource::fetchNextPlaylistItem()
{
    if (sigSrv->gotSigInt() || !mainQueue.hasNext())
    {
        // Playlist has no more items
        return NULL;
    }

    PlaylistAudioSourceItem* nextPlaylistItem = mainQueue.getNext();

    prepareNextTrack();

    currentTrack = nextPlaylistItem->getTrack();
    currentTrackStartTime = time(0);

    if (!nextPlaylistItem->readyToPlay())
    {
        nextPlaylistItem->waitToFinishEncode();
    }

    if (!nextPlaylistItem->isSuccessEncoded())
    {
        /* TODO: Should log this track as failed */
        logSrv->warn("Skipping: " + nextPlaylistItem->getTrack().getTrack());

        // Fetch next
        return fetchNextPlaylistItem();
    }

    return nextPlaylistItem;
}

bool PlaylistAudioSource::loadNextPlaylistItem()
{
    try
    {
        // Fetch next Playlist item
        PlaylistAudioSourceItem* nextPlaylistItem = fetchNextPlaylistItem();
        if (nextPlaylistItem == NULL)
        {
            return false;
        }

        // Set instances
        currentPlaylistItem = nextPlaylistItem;
        currentMp3File = openReadBinary(currentPlaylistItem->getTrackFile());
        logSrv->info("Playing: " + currentPlaylistItem->getTrackFile());

        // Raise audioMetedata Event
        string metadata = nextPlaylistItem->getTrack().getTrackTitle();
        AudioMetadataChangedEventArgs* args =
            new AudioMetadataChangedEventArgs(metadata);
        AudioMetadataChanged.raise(this, args);

        return true;
    }
    catch(DomainException& e)
    {
        logSrv->error(handle(e));

        // Raise errorAppeared Event
        ErrorAppeared.raise(this, NULL);

        // Clear current state
        finilizeCurrentPlayingTrack();

        // and suppress this error
        return false;
    }
}

void PlaylistAudioSource::prepareNextTrack()
{
    if (requestedTrackIndex.hasNext())
    {
        int trackIndex = requestedTrackIndex.getNext();
        PlaylistItem nextTrack = playlistHandler->getTrack(trackIndex);
        mainQueue.putBack(createPlaylistAudioSourceItem(nextTrack));
    }
    else if (playlistHandler->hasNext())
    {
        PlaylistItem nextTrack = playlistHandler->nextTrack();
        mainQueue.putBack(createPlaylistAudioSourceItem(nextTrack));
    }
}

void PlaylistAudioSource::finilizeCurrentPlayingTrack()
{
    if (currentMp3File != NULL)
    {
        fclose(currentMp3File);
        currentMp3File = NULL;
    }

    if (currentPlaylistItem != NULL)
    {
        archiveTrack(currentPlaylistItem);
        currentPlaylistItem = NULL;
    }
}

void PlaylistAudioSource::archiveTrack(PlaylistAudioSourceItem* item)
{
    playlistHandler->archiveTrack(item->getTrack());

    /* Return the encode thread back to pool */
    if (item->getEncodeThread() != NULL)
    {
        encodePool->putBack(item->getEncodeThread());
    }

    // /* Dispose the encode context */
    // if (item->getContext() != NULL)
    // {
    //     delete item->getContext();
    // }

    /* Dispose item reference */
    delete item;

    numberOfPlayedTracks++;
}

void PlaylistAudioSource::shutdownAudioSource()
{

}

void PlaylistAudioSource::initialize()
{
    playlistHandler = new PlaylistHandler(
        logSrv,
        config->playlistFilePath,
        config->historyFilePath,
        config->strategyType,
        config->repeat);

    logSrv->info("NoiseStreamer playlist initialized!");
    logSrv->info("NoiseStreamer playlist start loading..");

    playlistHandler->load();

    logSrv->info("Playlist: '" + config->playlistFilePath + "' loaded, with '" + numberToString<int>(playlistHandler->playlistSize()) + "' tracks");
    logSrv->info("History: '" + config->historyFilePath + "' loaded, with '" + numberToString<int>(playlistHandler->historySize()) + "' tracks");

    /* Put the first track into the queue */
    prepareNextTrack();

    /* Wait track to be encoded if needed */
    PlaylistAudioSourceItem* firstTrack = mainQueue.front();
    if (firstTrack != NULL)
    {
        firstTrack->waitToFinishEncode();
    }
}

int PlaylistAudioSource::readNextMp3Data(unsigned char* mp3OutBuffer, size_t buffer_size)
{
    if (sigSrv->gotSigInt() || isStoped())
    {
        // Finilize Mp3 File
        finilizeCurrentPlayingTrack();
        return 0;
    }

    if (currentMp3File == NULL || isGoToNext())
    {
        // Bring back to normal in case of goToNext
        resetNext();

        // load next if any
        bool hasNext = loadNextPlaylistItem();
        if (!hasNext)
        {
            logSrv->info("Playlist finished!");
            return 0;
        }
    }

    long read = fread(mp3OutBuffer, sizeof(char), buffer_size, currentMp3File);
    if (read > 0)
    {
        return read;
    }

    // Mp3 File finished
    finilizeCurrentPlayingTrack();

    // Read next
    return readNextMp3Data(mp3OutBuffer, buffer_size);
}

int PlaylistAudioSource::readNextEncodedMp3Data(unsigned char* mp3OutBuffer)
{
    return 0;
}
