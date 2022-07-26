#ifndef AgentMode_h__
#define AgentMode_h__

#include <string>
#include <noisekernel/Logger.h>
#include <noisekernel/Signal.h>
#include <noisekernel/Thread.h>
#include "../NoiseStreamer.h"

using namespace std;
using namespace NoiseKernel;

class AgentMode
{
private:
    volatile sig_atomic_t _exit;

    LogService *logSrv;
    SignalAdapter* sigAdapter;
    NoiseStreamer* noiseStreamer;
    Thread* th;

    string help();
    void startNoiseStreamerAsync();
    void stopNoiseStreamer();

    void processCommand(string command);

public:
    AgentMode(
        LogService *logSrv,
        SignalAdapter* sigAdapter,
        NoiseStreamer* noiseStreamer);
    virtual ~AgentMode();
};

#endif // AgentMode_h__
