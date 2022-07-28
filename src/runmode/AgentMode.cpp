#include "AgentMode.h"
#include <iostream>
#include <stdio.h>
#include "../exception/NoiseStreamerException.h"

using namespace std;

const char* AgentMode::START = "start";

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
    taskRunner = new NoiseStreamerTaskRunner();
}

AgentMode::~AgentMode()
{
    if (th != NULL)
    {
        th->wait();
        delete th;
    }

    delete taskRunner;
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

void AgentMode::initialize()
{
    TcpServer::initialize();

    startNoiseStreamerAsync();
}

bool AgentMode::validateCommand(string command)
{
    if (TcpServer::validateCommand(command))
	{
		return true;
	}

    if (command == START)
    {
        return true;
    }

	return taskRunner->taskExist(command);
}

void AgentMode::processCommand(TcpClientConnection *client, string command)
{
    logSrv->trace("Proccess command: " + command);

    TcpStream *stream = client->getStream();

    string strValue = command;

    try
    {
        void* retval = NULL;

        if (command == START)
        {
            retval = startNoiseStreamerAsync();
        }
        else
        {
            retval = taskRunner->runNoiseStreamerTask(command, validStreamer());
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

void AgentMode::processErrorCommand(TcpClientConnection *client, string command)
{
    TcpServer::processErrorCommand(client, command);
    logSrv->error("Not found command: " + command);
}
