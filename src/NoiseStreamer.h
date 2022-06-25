#ifndef NoiseStreamer_h__
#define NoiseStreamer_h__

#include <string>
#include <noisekernel/Logger.h>
#include <noisekernel/Signal.h>

#include "config/NoiseStreamerConfig.h"

using namespace std;
using namespace NoiseKernel;

class NoiseStreamer
{
private:
    static const char* USER_AGENT;

    LogService* logSrv;
    SignalAdapter* sigAdapter;

    NoiseStreamerConfig* config;

public:
    NoiseStreamer(
        LogService *logSrv,
        SignalAdapter* sigAdapter,
        NoiseStreamerConfig* config
    );
    virtual ~NoiseStreamer();
};

#endif // NoiseStreamer_h__
