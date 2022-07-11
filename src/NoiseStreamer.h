#ifndef NoiseStreamer_h__
#define NoiseStreamer_h__

#include <string>
#include <noisekernel/Logger.h>
#include <noisekernel/Signal.h>

#include "libshout/LibShout.h"
#include "config/NoiseStreamerConfig.h"
#include "audio/source/AudioSource.h"
#include "health/NoiseStreamerHealthPolicy.h"
#include "audio/encode/NoiseStreamerEncoder.h"

using namespace std;
using namespace NoiseKernel;

class NoiseStreamer
{
    class ErrorAppearedEventHandler: public EventHandler
    {
    private:
        NoiseStreamer *noiseStreamer;
    public:
        ErrorAppearedEventHandler(NoiseStreamer *noiseStreamer);
        virtual ~ErrorAppearedEventHandler();
        virtual void onEvent(void* sender, EventArgs* e);
    };

    class AudioMetadataChangedEventHandler: public EventHandler
    {
    private:
        NoiseStreamer *noiseStreamer;
    public:
        AudioMetadataChangedEventHandler(NoiseStreamer *noiseStreamer);
        virtual ~AudioMetadataChangedEventHandler();
        virtual void onEvent(void* sender, EventArgs* e);
    };

private:
    static const char* USER_AGENT;

    LogService* logSrv;
    SignalAdapter* sigAdapter;
    NoiseStreamerConfig* config;
    AudioSource* audioSource;
    NoiseStreamerHealthPolicy* healthPolicy;

    LibShout* libShout;
    AudioMetadataChangedEventHandler* audioMetadataChangedEventHandler;
    ErrorAppearedEventHandler* errorAppearedEventHandler;
    NoiseStreamerEncoder* encoder;
    NoiseStreamerEncoder* decoder;

    void initializeShout();
    void connectShout();
	void finilizeShout();
    void streamAudioSource();

public:
    NoiseStreamer(
        LogService *logSrv,
        SignalAdapter* sigAdapter,
        NoiseStreamerConfig* config,
        AudioSource* audioSource,
        NoiseStreamerHealthPolicy* healthPolicy);
    virtual ~NoiseStreamer();

    string userAgent();

    void initialize();
    void connect();
    void disconnect();
    void shutdown();
    void stream();
};

#endif // NoiseStreamer_h__
