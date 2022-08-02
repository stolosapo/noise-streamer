#include "AgentProtocol.h"

AgentProtocol::AgentProtocol(bool isServer): TcpProtocol(isServer)
{

}

AgentProtocol::~AgentProtocol()
{

}

AgentProtocol* AgentProtocol::createForServer()
{
    return new AgentProtocol(true);
}

AgentProtocol* AgentProtocol::createForClient()
{
    return new AgentProtocol(false);
}

string AgentProtocol::prompt()
{
    return "streamer> ";
}
