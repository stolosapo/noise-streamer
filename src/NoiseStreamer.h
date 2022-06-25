#ifndef NoiseStreamer_h__
#define NoiseStreamer_h__

#include <string>
#include <noisekernel/Logger.h>
#include <noisekernel/Signal.h>

using namespace std;
using namespace NoiseKernel;

class NoiseStreamer
{
private:
    static const char* USER_AGENT;

    LogService* logSrv;
    SignalAdapter* sigAdapter;

public:
    NoiseStreamer(
        LogService *logSrv,
        SignalAdapter* sigAdapter
    );
    virtual ~NoiseStreamer();
};

#endif // NoiseStreamer_h__
