#include "AgentMode.h"
#include <iostream>
#include <stdio.h>
#include "AgentModeTask.h"
#include "../NoiseStreamerTask.h"
#include "../exception/NoiseStreamerException.h"
#include "../utils/StringHelper.h"

using namespace std;

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
    streamerTaskRunner = createNoiseStreamerTaskRunner();
    runner = createAgentModeTaskRunner();
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

    runner->runTask("start", this);
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
            retval = streamerTaskRunner->runTask(command, validStreamer());
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
