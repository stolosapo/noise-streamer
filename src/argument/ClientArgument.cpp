#include "ClientArgument.h"
#include "../exception/NoiseStreamerException.h"

const string ClientArgument::AGENTHOSTNAME = "agenthostname";
const string ClientArgument::AGENTPORT = "agentport";

ClientArgument::ClientArgument(ArgumentProvider* argProvider): ArgumentAdapter(argProvider)
{

}

ClientArgument::~ClientArgument()
{

}

string ClientArgument::title()
{
    return "Client args";
}

void ClientArgument::registerArguments()
{
    registerArg(AGENTHOSTNAME, "The hostname of NoisStreamer Agent to connect. Default: 'localhost'");
    registerArg(AGENTPORT, "The port of NoisStreamer Agent to connect. Default: '51717'");
}

string ClientArgument::getAgentHostName()
{
    if (!hasArg(AGENTHOSTNAME))
    {
        return "localhost";
    }

    string s = getStringValue(AGENTHOSTNAME);
    if (s == "")
    {
        throw DomainException(ARG0001, AGENTHOSTNAME);
    }
    return s;
}

int ClientArgument::getAgentPort()
{
    if (!hasArg(AGENTPORT))
    {
        return 51717;
    }
    return getIntValue(AGENTPORT);
}
