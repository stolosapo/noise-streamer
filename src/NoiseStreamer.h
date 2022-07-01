#ifndef NoiseStreamer_h__
#define NoiseStreamer_h__

#include <string>
#include <noisekernel/Logger.h>
#include <noisekernel/Signal.h>

#include "libshout/LibShout.h"
#include "config/NoiseStreamerConfig.h"
#include "audio/source/AudioSource.h"

using namespace std;
using namespace NoiseKernel;

class NoiseStreamer
{
private:
    static const char* USER_AGENT;

    LogService* logSrv;
    SignalAdapter* sigAdapter;
    NoiseStreamerConfig* config;

    LibShout* libShout;
    AudioSource* audioSource;

    void initializeShout();
    void connectShout();
	void finilizeShout();
    void streamAudioSource();

public:
    NoiseStreamer(
        LogService *logSrv,
        SignalAdapter* sigAdapter,
        NoiseStreamerConfig* config,
        AudioSource* audioSource);
    virtual ~NoiseStreamer();

    string userAgent();

    void initialize();
    void connect();
    void disconnect();
    void shutdown();
    void stream();
};

#endif // NoiseStreamer_h__
