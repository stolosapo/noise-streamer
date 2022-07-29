#include "AgentMode.h"
#include <iostream>
#include <stdio.h>
#include "../exception/NoiseStreamerException.h"
#include "../utils/StringHelper.h"

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
    streamerTaskRunner = new NoiseStreamerTaskRunner;
    runner = new TaskRunner;
    runner->registerTask(START, &agent_start_streamer);
    runner->registerTask("agent-status", &agent_status);
}

AgentMode::~AgentMode()
{
    if (th != NULL)
    {
        th->wait();
        delete th;
    }

    delete streamerTaskRunner;
    delete runner;
}


NoiseStreamer* AgentMode::validStreamer()
{
    if (th != NULL && !th->isRunning())
    {
        throw RuntimeException("NoiseStreamer is not running");
    }
    return noiseStreamer;
}

void AgentMode::initialize()
{
    TcpServer::initialize();

    runner->runTask(START, this);
}

bool AgentMode::validateCommand(string command)
{
    if (!TcpServer::validateCommand(command))
	{
		return false;
	}

    if (runner->taskExist(command))
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

        if (runner->taskExist(command))
        {
            retval = runner->runTask(command, this);
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
