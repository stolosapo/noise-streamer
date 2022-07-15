#ifndef InteractiveMode_h__
#define InteractiveMode_h__

#include <string>
#include <noisekernel/Logger.h>
#include <noisekernel/Signal.h>
#include <noisekernel/Thread.h>
#include "../NoiseStreamer.h"
#include "../NoiseStreamerTask.h"

using namespace std;
using namespace NoiseKernel;

class InteractiveMode: public TaskRunner
{
private:
    static const char* PROMPT;
    volatile sig_atomic_t _exit;

    LogService *logSrv;
    SignalAdapter* sigAdapter;
    NoiseStreamer* noiseStreamer;

    static void* help(void*);

    void registerTasks();
    void processCommand(string command);

public:
    InteractiveMode(
        LogService *logSrv,
        SignalAdapter* sigAdapter,
        NoiseStreamer* noiseStreamer);
    virtual ~InteractiveMode();

    void start();
};

#endif // InteractiveMode_h__
