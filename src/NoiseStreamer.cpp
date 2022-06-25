#include "NoiseStreamer.h"
#include "utils/StringHelper.h"

const char* NoiseStreamer::USER_AGENT = "noisestreamer";

NoiseStreamer::NoiseStreamer(
    LogService *logSrv,
    SignalAdapter* sigAdapter,
    NoiseStreamerConfig* config
): logSrv(logSrv), sigAdapter(sigAdapter), config(config)
{
    audioSource = NULL;
}

NoiseStreamer::~NoiseStreamer()
{
    if (audioSource != NULL)
    {
        delete audioSource;
    }

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

    libShout->setAudioInfoBitrate(config->bitrate);
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
