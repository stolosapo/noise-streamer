#include "AgentArgument.h"
#include "../exception/NoiseStreamerException.h"

const string AgentArgument::SERVERHOSTNAME = "serverhostname";
const string AgentArgument::SERVERPORT = "serverport";
const string AgentArgument::SERVERPOOLSIZE = "serverpoolsize";

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
    registerArg(SERVERHOSTNAME, "The hostname of NoisStreamer Agent Server. Default: 'localhost'");
    registerArg(SERVERPORT, "The port of NoisStreamer Agent Server. Default: '51717'");
    registerArg(SERVERPOOLSIZE, "The pool size of NoisStreamer Agent Server. Default: '5'");
}

string AgentArgument::getServerHostName()
{
    if (!hasArg(SERVERHOSTNAME))
    {
        return "localhost";
    }

    string s = getStringValue(SERVERHOSTNAME);
    if (s == "")
    {
        throw DomainException(ARG0001, SERVERHOSTNAME);
    }
    return s;
}

int AgentArgument::getServerPort()
{
    if (!hasArg(SERVERPORT))
    {
        return 51717;
    }
    return getIntValue(SERVERPORT);
}

int AgentArgument::getServerPoolSize()
{
    if (!hasArg(SERVERPOOLSIZE))
    {
        return 5;
    }

    int s = getIntValue(SERVERPOOLSIZE);
    if (s == 0)
    {
        throw DomainException(ARG0001, SERVERPOOLSIZE);
    }
    return s;
}
