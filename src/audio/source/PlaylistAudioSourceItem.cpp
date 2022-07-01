#include "PlaylistAudioSourceItem.h"

#include "../../exception/NoiseStreamerException.h"
#include "../../utils/StringHelper.h"
#include "../../utils/FileHelper.h"

PlaylistAudioSourceItem::PlaylistAudioSourceItem(PlaylistItem track)
    : track(track), needEncode(false), successEncoded(true)
{
    PlaylistAudioSourceItemContext emptyContext;
    emptyContext.encodeThread = NULL;
    emptyContext.logSrv = NULL;
    context = emptyContext;
}

PlaylistAudioSourceItem::PlaylistAudioSourceItem(PlaylistItem track, PlaylistAudioSourceItemContext context)
    : track(track), needEncode(true), successEncoded(false), encodedTrackFile(""), context(context)
{

}

PlaylistAudioSourceItem::~PlaylistAudioSourceItem()
{

}

PlaylistItem PlaylistAudioSourceItem::getTrack()
{
    return track;
}

Thread* PlaylistAudioSourceItem::getEncodeThread()
{
    return context.encodeThread;
}

string PlaylistAudioSourceItem::getTrackFile() const
{
    if (needEncode)
    {
        return encodedTrackFile;
    }

    return track.getTrack();
}

PlaylistAudioSourceItemContext PlaylistAudioSourceItem::getContext()
{
    return context;
}

bool PlaylistAudioSourceItem::isSuccessEncoded()
{
    return successEncoded;
}

bool PlaylistAudioSourceItem::readyToPlay()
{
    Thread* th = getEncodeThread();
    if (th == NULL)
    {
        return true;
    }

    return !th->isRunning();
}

void PlaylistAudioSourceItem::prepare()
{
    if (!needEncode)
    {
        return;
    }

    Thread* th = getEncodeThread();
    if (th == NULL)
    {
        return;
    }

    th->attachDelegate(&encodeTrack);
    th->start(this, "runner: NoiseStreamerPlaylist, re-encode track");
}

void PlaylistAudioSourceItem::waitToFinishEncode()
{
    Thread* th = getEncodeThread();
    if (th == NULL)
    {
        return;
    }

    th->wait();
}

string PlaylistAudioSourceItem::reencode()
{
    string mp3InPath = track.getTrack();

    if (context.logSrv == NULL || context.encodeThread == NULL)
    {
        throw DomainException(NSS0024, mp3InPath);
    }

    LogService* logSrv = context.logSrv;
    // AudioEncodingService* encSrv = context->getEncSrv();

    string pcmOutPath = context.pcmOutPath;
    string mp3OutPath = context.mp3OutPath;
    int threadPoolIndex = context.encodeThread->getPoolIndex();
    string poolIndex = numberToString<int>(threadPoolIndex);

    string pcmAudioFile = pcmOutPath + "pcm_audio_" + poolIndex + ".wav";
    string mp3AudioFile = mp3OutPath + "mp3_audio_" + poolIndex + ".mp3";

    logSrv->info("Start Re-Encoding.. '" + mp3InPath + "'");

	// encSrv->decode(mp3InPath, pcmAudioFile);

    logSrv->info("Decoded.. '" + mp3InPath + "' to '" + pcmAudioFile + "'");

    AudioTag* metadata = track.getMetadata();
    metadata->setReencodeData(
        context.encodeMode,
        context.audioBitrate,
        context.samplerate,
        context.quality);

	// encSrv->encode(pcmAudioFile, mp3AudioFile, metadata);

    logSrv->info("End Re-Encoding.. '" + mp3InPath + "'");

    // TODO: Fix this
    // return mp3AudioFile;
    return mp3InPath;
}

void* PlaylistAudioSourceItem::encodeTrack(void* context)
{
    PlaylistAudioSourceItem* item = (PlaylistAudioSourceItem*) context;

    if (item->context.logSrv == NULL)
    {
        throw DomainException(NSS0024, item->track.getTrack());
    }

    LogService* logSrv = item->context.logSrv;

    try
    {
        string encodedFile = item->reencode();

        // Check if file is valid
        if (exists(encodedFile.c_str()))
        {
            item->encodedTrackFile = encodedFile;
            item->successEncoded = true;
        }
    }
    catch (DomainException& e)
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

    return NULL;
}
