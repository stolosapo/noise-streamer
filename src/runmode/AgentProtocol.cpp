#include "AgentProtocol.h"

AgentProtocol::AgentProtocol(bool isServer): TcpProtocol(isServer)
{

}

AgentProtocol::~AgentProtocol()
{

}

string AgentProtocol::prompt()
{
    return "streamer> ";
}
