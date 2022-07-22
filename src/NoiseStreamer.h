#ifndef NoiseStreamer_h__
#define NoiseStreamer_h__

#include <string>
#include <noisekernel/Logger.h>
#include <noisekernel/Signal.h>
#include <noisekernel/Thread.h>

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
    volatile sig_atomic_t _stop;

    LogService* logSrv;
    SignalAdapter* sigAdapter;
    NoiseStreamerConfig* config;
    AudioSource* audioSource;
    NoiseStreamerHealthPolicy* healthPolicy;

    LibShout* libShout;
    AudioMetadataChangedEventHandler* audioMetadataChangedEventHandler;
    ErrorAppearedEventHandler* errorAppearedEventHandler;
    NoiseStreamerEncoder* encoder;

    static void* startStreamerThread(void* noiseStreamer);

    void initializeShout();
    void connectShout();
	void finilizeShout();
    void streamAudioSource();

    void initialize();
    void connect();
    void disconnect();
    void shutdown();
    void stream();

public:
    NoiseStreamer(
        LogService *logSrv,
        SignalAdapter* sigAdapter,
        NoiseStreamerConfig* config,
        AudioSource* audioSource,
        NoiseStreamerHealthPolicy* healthPolicy);
    virtual ~NoiseStreamer();

    string userAgent();

    void start();
    Thread* startAsync();
    void stop();
};

#endif // NoiseStreamer_h__
