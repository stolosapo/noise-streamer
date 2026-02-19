#include "NoiseStreamer.h"
#include "exception/NoiseStreamerException.h"
#include "audio/source/AudioMetadataChangedEventArgs.h"
#include "audio/encode/EncodeContext.h"
#include "utils/StringHelper.h"

using namespace NoiseKernel;

const char* NoiseStreamer::USER_AGENT = "noisestreamer";

NoiseStreamer::NoiseStreamer(
    LogService *logSrv,
    SignalAdapter* sigAdapter,
    NoiseStreamerConfig* config,
    PlaylistAudioSourceConfig* playlistConfig,
    AudioSource* audioSource,
    NoiseStreamerHealthPolicy* healthPolicy)
    : logSrv(logSrv),
    sigAdapter(sigAdapter),
    config(config),
    playlistConfig(playlistConfig),
    audioSource(audioSource),
    healthPolicy(healthPolicy)
{
    _stop = 1;
    libShout = NULL;
    audioMetadataChangedEventHandler = NULL;
    errorAppearedEventHandler = NULL;
    encoder = NULL;

    // Initialize Audio Source
    audioMetadataChangedEventHandler = new AudioMetadataChangedEventHandler(this);
    errorAppearedEventHandler = new ErrorAppearedEventHandler(this);
    audioSource->AudioMetadataChanged += audioMetadataChangedEventHandler;
    audioSource->ErrorAppeared += errorAppearedEventHandler;

    playlistSource = new PlaylistSource(logSrv, sigAdapter);
    playlistSource->AudioMetadataChanged += audioMetadataChangedEventHandler;
    playlistSource->ErrorAppeared += errorAppearedEventHandler;
}

NoiseStreamer::~NoiseStreamer()
{
    finilizeShout();

    if (audioMetadataChangedEventHandler != NULL)
    {
        delete audioMetadataChangedEventHandler;
    }

    if (errorAppearedEventHandler != NULL)
    {
        delete errorAppearedEventHandler;
    }

    if (encoder != NULL)
    {
        delete encoder;
    }

    if (playlistSource != NULL)
    {
        delete playlistSource;
    }
}

NoiseStreamer::ErrorAppearedEventHandler::ErrorAppearedEventHandler(NoiseStreamer *noiseStreamer)
    : EventHandler(), noiseStreamer(noiseStreamer)
{

}

NoiseStreamer::ErrorAppearedEventHandler::~ErrorAppearedEventHandler()
{

}

void NoiseStreamer::ErrorAppearedEventHandler::onEvent(void* sender, EventArgs* e)
{
    noiseStreamer->healthPolicy->incrementErrorCounter();

    if (e != NULL)
    {
        delete e;
    }
}

NoiseStreamer::AudioMetadataChangedEventHandler::AudioMetadataChangedEventHandler(NoiseStreamer *noiseStreamer)
    : EventHandler(), noiseStreamer(noiseStreamer)
{

}

NoiseStreamer::AudioMetadataChangedEventHandler::~AudioMetadataChangedEventHandler()
{

}

void NoiseStreamer::AudioMetadataChangedEventHandler::onEvent(void* sender, EventArgs* e)
{
    if (e == NULL)
    {
        return;
    }

    AudioMetadataChangedEventArgs* eventArgs = (AudioMetadataChangedEventArgs*) e;

    try
    {
        string metadata = eventArgs->getMetadata();
        if (noiseStreamer->libShout == NULL)
        {
            noiseStreamer->logSrv->warn("Trying to update metadata on NULL libshout");
            return;
        }

        noiseStreamer->libShout->updateMetadata(metadata);
    }
    catch(DomainException& e)
    {
        noiseStreamer->logSrv->error(handle(e));
    }
    catch(RuntimeException& e)
    {
        noiseStreamer->logSrv->error(handle(e));
    }
    catch(exception& e)
    {
        noiseStreamer->logSrv->error(e.what());
    }

    delete eventArgs;
}

string NoiseStreamer::userAgent()
{
    // TODO: this should be variable
    return string(USER_AGENT) + "/1.0.0";
}

AudioSource* NoiseStreamer::getAudioSource()
{
    return audioSource;
}

void NoiseStreamer::initializeShout()
{
    finilizeShout();

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
    const int ENCODE_AUDIO_SIZE = NoiseStreamerEncoder::MP3_SIZE * 10;
    short read_buffer[2048];
    unsigned char mp3EncodedBuffer[ENCODE_AUDIO_SIZE];

    int encodeWrite;

    playlistSource->start();

    while (!sigAdapter->gotSigInt() && _stop != 1)
    {
        healthPolicy->assertErrorCounterThresholdReached();

        size_t read = playlistSource->readOutput(read_buffer, 2048);

        /*
         * return code     number of bytes output in mp3buf. Can be 0
         *                 -1:  mp3buf was too small
         *                 -2:  malloc() problem
         *                 -3:  lame_init_params() not called
         *                 -4:  psycho acoustic problems
        */
        encodeWrite = encoder->encode(read_buffer, 1024, mp3EncodedBuffer, ENCODE_AUDIO_SIZE);
        if (encodeWrite < 0)
        {
            logSrv->warn("Could not encode sample, returned " + numberToString<int>(encodeWrite));
            // TODO: Instead of stopping loop, maybe is better to send interupt to audioSource to go to next
            // and the continue loop
            break;
        }

        if (!libShout->isConnected())
        {
            string connStr = numberToString<int>(libShout->getConnected());
            throw DomainException(NSS0019, "Connection status '" + connStr + "'");
        }

        libShout->shoutSend(mp3EncodedBuffer, encodeWrite);

        healthPolicy->setShoutQueueLenth(libShout->shoutQueuelen());
        healthPolicy->assertErrorCounterThresholdReached();

        libShout->shoutSync();
    }

    logSrv->info("AudioSource streamming stopped");
}

void NoiseStreamer::initialize()
{
    // Initialize Audio Source
    playlistSource->initialize(*playlistConfig);

    // Initialize Encoder
    EncodeContext context;
    context.bitrate = (int) config->bitrate;
    context.samplerate = stringToNumber<int>(config->samplerate);
    context.encodeMode = CBR;
    context.quality = 3;

    if (encoder != NULL)
    {
        delete encoder;
    }

    encoder = new NoiseStreamerEncoder;
    encoder->initForEncode(&context);
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
    playlistSource->stop();
    encoder->finilizeEncode();
    logSrv->info("NoiseStreamer is shutted down!");
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

void NoiseStreamer::start()
{
    if (_stop == 0)
    {
        logSrv->warn("NoiseStreamer is already started!, skipping");
        return;
    }

    _stop = 0;

    initialize();
    connect();
    stream();
    disconnect();
    shutdown();

    _stop = 1;
}

void* NoiseStreamer::startStreamerThread(void* noiseStreamer)
{
    NoiseStreamer* ns = (NoiseStreamer*) noiseStreamer;
    ns->start();
    return NULL;
}

Thread* NoiseStreamer::startAsync()
{
    Thread* th = new Thread;
    th->attachDelegate(&NoiseStreamer::startStreamerThread);
    th->start(this);
    return th;
}

void NoiseStreamer::stop()
{
    if (_stop == 1)
    {
        logSrv->warn("NoiseStreamer is already stopped!, skipping");
    }

    _stop = 1;
}
