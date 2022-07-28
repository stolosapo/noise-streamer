#ifndef AgentMode_h__
#define AgentMode_h__

#include <string>
#include <noisekernel/Logger.h>
#include <noisekernel/Signal.h>
#include <noisekernel/Thread.h>
#include <noisekernel/Tcp.h>
#include "../NoiseStreamer.h"
#include "../NoiseStreamerTaskRunner.h"

using namespace std;
using namespace NoiseKernel;

class AgentMode: public TcpServer
{
private:
    static const char* START;
    static const char* AGENT_STATUS;

    LogService* logSrv;
    SignalAdapter* sigSrv;
    NoiseStreamer* noiseStreamer;
    NoiseStreamerTaskRunner* streamerTaskRunner;
    Thread* th;

    NoiseStreamer* validStreamer();
    void* startNoiseStreamerAsync();
    void* agentStatus();

protected:
    virtual void initialize();
    virtual bool validateCommand(string command);
    virtual void processCommand(TcpClientConnection *client, string command);

public:
    AgentMode(
        LogService *logSrv,
        SignalAdapter *sigSrv,
        TcpServerConfig *config,
        TcpProtocol *protocol,
        NoiseStreamer* noiseStreamer);
    virtual ~AgentMode();
};

#endif // AgentMode_h__
