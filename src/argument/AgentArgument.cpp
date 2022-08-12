#include "AgentArgument.h"
#include "../exception/NoiseStreamerException.h"

const string AgentArgument::AGENTHOSTNAME = "agenthostname";
const string AgentArgument::AGENTPORT = "agentport";
const string AgentArgument::AGENTPOOLSIZE = "agentpoolsize";

AgentArgument::AgentArgument(ArgumentProvider* argProvider): ArgumentAdapter(argProvider)
{

}

AgentArgument::~AgentArgument()
{

}

string AgentArgument::title()
{
    return "Agent args";
}

void AgentArgument::registerArguments()
{
    registerArg(AGENTHOSTNAME, "The hostname of NoisStreamer Agent. Default: 'localhost'");
    registerArg(AGENTPORT, "The port of NoisStreamer Agent. Default: '51717'");
    registerArg(AGENTPOOLSIZE, "The pool size of NoisStreamer Agent. Default: '5'");
}

string AgentArgument::getAgentHostName()
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

int AgentArgument::getAgentPort()
{
    if (!hasArg(AGENTPORT))
    {
        return 51717;
    }
    return getIntValue(AGENTPORT);
}

int AgentArgument::getAgentPoolSize()
{
    if (!hasArg(AGENTPOOLSIZE))
    {
        return 5;
    }

    int s = getIntValue(AGENTPOOLSIZE);
    if (s == 0)
    {
        throw DomainException(ARG0001, AGENTPOOLSIZE);
    }
    return s;
}
