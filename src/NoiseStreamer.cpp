#include "NoiseStreamer.h"
#include "exception/NoiseStreamerException.h"
#include "audio/source/PlaylistAudioSource.h"
#include "utils/StringHelper.h"

using namespace NoiseKernel;

const char* NoiseStreamer::USER_AGENT = "noisestreamer";

NoiseStreamer::NoiseStreamer(
    LogService *logSrv,
    SignalAdapter* sigAdapter,
    NoiseStreamerConfig* config,
    AudioSource* audioSource,
    NoiseStreamerHealthPolicy* healthPolicy)
    : logSrv(logSrv),
    sigAdapter(sigAdapter),
    config(config),
    audioSource(audioSource),
    healthPolicy(healthPolicy)
{
    libShout = NULL;
}

NoiseStreamer::~NoiseStreamer()
{
    finilizeShout();
}

string NoiseStreamer::userAgent()
{
    // TODO: this should be variable
    return string(USER_AGENT) + "/1.0.0";
}

void NoiseStreamer::initializeShout()
{
    libShout = new LibShout(logSrv, sigAdapter);
    libShout->initializeShout();

    string shoutVersion = libShout->shoutFullVersion();
    string lameVersion = ""; // encSrv->version();
    string fullVersion = userAgent() + " " + shoutVersion + " " + lameVersion;

    libShout->setAgent(fullVersion);
    libShout->setProtocolHttp();
    libShout->setHost(config->hostname);
    libShout->setPort(stringToNumber<unsigned short>(config->port));
    libShout->setMount(config->mountpoint);

    libShout->setUser(config->username);
    libShout->setPassword(config->password);

    libShout->setFormatMp3();
    libShout->setPublic(stringToNumber<unsigned int>(config->isPublic));
    libShout->setNonblocking(1);

    libShout->setName(config->name);
    libShout->setUrl(config->url);
    libShout->setGenre(config->genre);
    libShout->setDescription(config->description);

    libShout->setAudioInfoBitrate(convertToString(config->bitrate));
    libShout->setAudioInfoSamplerate(config->samplerate);
    libShout->setAudioInfoChannels(config->channels);

    logSrv->info("LibShout initialized: " + shoutVersion);
}

void NoiseStreamer::connectShout()
{
    libShout->startShout();
}

void NoiseStreamer::finilizeShout()
{
    if (libShout != NULL)
    {
        libShout->finilizeShout();
        delete libShout;
        libShout = NULL;
    }
}

void NoiseStreamer::streamAudioSource()
{
    const int AUDIO_SIZE = 4096;

    unsigned char buff[AUDIO_SIZE * 100];
    long read;

    while (!sigAdapter->gotSigInt())
    {
        healthPolicy->assertErrorCounterThresholdReached();

        read = audioSource->readNextMp3Data(buff, AUDIO_SIZE);
        // read = ((PlaylistAudioSource*) audioSource)->readNextEncodedMp3Data(buff);
        if (read <= 0)
        {
            break;
        }

        if (!libShout->isConnected())
        {
            string connStr = numberToString<int>(libShout->getConnected());
            throw DomainException(NSS0019, "Connection status '" + connStr + "'");
        }

        libShout->shoutSend(buff, read);

        healthPolicy->setShoutQueueLenth(libShout->shoutQueuelen());
        healthPolicy->assertErrorCounterThresholdReached();

        libShout->shoutSync();
    }
}

void NoiseStreamer::initialize()
{
    // Initialize Audio Source
    // audioSource->AudioMetadataChanged += audioMetadataChangedEventHandler;
    // audioSource->ErrorAppeared += errorAppearedEventHandler;
    audioSource->initialize();
}

void NoiseStreamer::connect()
{
    initializeShout();

    connectShout();
}

void NoiseStreamer::disconnect()
{
    finilizeShout();
}

void NoiseStreamer::shutdown()
{
    audioSource->shutdownAudioSource();
}

void NoiseStreamer::stream()
{
    try
    {
        healthPolicy->resetErrorCounter();

        streamAudioSource();
    }
    catch(DomainException& e)
    {
        logSrv->error(handle(e));
    }
}
