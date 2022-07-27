#include "AgentMode.h"
#include <iostream>
#include <stdio.h>
#include "../exception/NoiseStreamerException.h"

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
}

AgentMode::~AgentMode()
{
    if (th != NULL)
    {
        th->wait();
        delete th;
    }
}

void AgentMode::startNoiseStreamerAsync()
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
    cout << "NoiseStreamer started streaming!" << endl;
}

void AgentMode::stopNoiseStreamer()
{
    if (th != NULL && !th->isRunning())
    {
        throw RuntimeException("NoiseStreamer is already stopped!");
    }

    noiseStreamer->stop();
    cout << "NoiseStreamer stopped streaming!" << endl;
}

string AgentMode::help()
{
    return "Some help..";
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

	return true;
}

void AgentMode::processCommand(TcpClientConnection *client, string command)
{
    TcpStream *stream = client->getStream();

    string strValue = command;

    try
    {
    	// void* retval = agentProtocol()->runParametrizedTask(command, this);

    	// if (retval != NULL)
    	// {
    	// 	string *str = static_cast<string*>(retval);
    	// 	strValue = *str;

    	// 	delete str;
    	// }
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
    logSrv->error("");
}

// void AgentMode::processCommand(string command)
// {
//     try
//     {

//         throw RuntimeException("Command not found, try 'help'");
//     }
//     catch (DomainException &e)
//     {
//         cerr << handle(e) << endl;
//     }
//     catch (RuntimeException &e)
//     {
//         cerr << handle(e) << endl;
//     }
//     catch (exception &e)
//     {
//         cerr << e.what() << endl;
//     }
// }
