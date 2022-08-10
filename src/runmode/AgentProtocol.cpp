#include "AgentProtocol.h"
#include "AgentModeTask.h"

AgentProtocol::AgentProtocol(bool isServer): TcpProtocol(isServer)
{

}

AgentProtocol::~AgentProtocol()
{

}

string AgentProtocol::help()
{
    return createAgentModeTaskRunnerHelp();
}

string AgentProtocol::prompt()
{
    return "streamer> ";
}
