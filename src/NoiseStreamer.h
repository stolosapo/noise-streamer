#ifndef NoiseStreamer_h__
#define NoiseStreamer_h__

#include <string>
#include <noisekernel/Logger.h>
#include <noisekernel/Signal.h>

#include "libshout/LibShout.h"
#include "config/NoiseStreamerConfig.h"
#include "audio/AudioSource.h"

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

public:
    NoiseStreamer(
        LogService *logSrv,
        SignalAdapter* sigAdapter,
        NoiseStreamerConfig* config
    );
    virtual ~NoiseStreamer();

    string userAgent();
};

#endif // NoiseStreamer_h__
