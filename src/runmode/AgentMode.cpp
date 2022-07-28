#include "AgentMode.h"
#include <iostream>
#include <stdio.h>
#include "../exception/NoiseStreamerException.h"
#include "../utils/StringHelper.h"

using namespace std;

const char* AgentMode::START = "start";
const char* AgentMode::AGENT_STATUS = "agent-status";

AgentMode::AgentMode(
    LogService *logSrv,
    SignalAdapter *sigSrv,
    TcpServerConfig *config,
    TcpProtocol *protocol,
    NoiseStreamer* noiseStreamer)
    : TcpServer(logSrv, sigSrv, config, protocol),
    logSrv(logSrv),
    sigSrv(sigSrv),
    noiseStreamer(noiseStreamer)
{
    th = NULL;
    streamerTaskRunner = new NoiseStreamerTaskRunner;
}

AgentMode::~AgentMode()
{
    if (th != NULL)
    {
        th->wait();
        delete th;
    }

    delete streamerTaskRunner;
}

NoiseStreamer* AgentMode::validStreamer()
{
    if (th != NULL && !th->isRunning())
    {
        throw RuntimeException("NoiseStreamer is not running");
    }
    return noiseStreamer;
}

void* AgentMode::startNoiseStreamerAsync()
{
    if (th != NULL && th->isRunning())
    {
        throw RuntimeException("NoiseStreamer is already running!");
    }

    if (th != NULL)
    {
        th->wait();
        delete th;
    }

    th = noiseStreamer->startAsync();
    return static_cast<void*>(new string("NoiseStreamer started streaming!"));
}

void* AgentMode::agentStatus()
{
    double uptimeSec = uptime();
	int connections = numberOfActiveConnections();

	int sec = uptimeSec;

	int days = sec / 60 / 60 / 24;
	int hours = (sec / 60 / 60) % 24;
	int minutes = (sec / 60) % 60;
	int seconds = sec % 60;

	char s[25];
	sprintf(s, "%01d days, %02d:%02d:%02d", days, hours, minutes, seconds);
	string str(s);

	string value = "\n";

	// value += "Version: " + string(client->version()) + "\n";
	value += "Uptime: " + str + "\n";
	value += "Active connections: " + numberToString<int>(connections) + "\n";

	return static_cast<void*>(new string(value));
}

void AgentMode::initialize()
{
    TcpServer::initialize();

    startNoiseStreamerAsync();
}

bool AgentMode::validateCommand(string command)
{
    if (!TcpServer::validateCommand(command))
	{
		return false;
	}

    if (command == START || command == AGENT_STATUS)
    {
        return true;
    }

	return streamerTaskRunner->taskExist(command);
}

void AgentMode::processCommand(TcpClientConnection *client, string command)
{
    TcpStream *stream = client->getStream();

    string strValue = command;

    try
    {
        void* retval = NULL;

        if (command == START)
        {
            retval = startNoiseStreamerAsync();
        }
        else if (command == AGENT_STATUS)
        {
            retval = agentStatus();
        }
        else
        {
            retval = streamerTaskRunner->runNoiseStreamerTask(command, validStreamer());
        }

        if (retval != NULL)
    	{
    		string *str = static_cast<string*>(retval);
    		strValue = *str;
    		delete str;
    	}
    }
    catch(DomainException& e)
    {
        strValue = handle(e);
        logSrv->error(strValue);
    }
    catch(RuntimeException& e)
    {
        strValue = handle(e);
        logSrv->error(strValue);
    }
    catch(exception& e)
    {
        strValue = e.what();
        logSrv->error(strValue);
    }

	stream->send(strValue);
}
