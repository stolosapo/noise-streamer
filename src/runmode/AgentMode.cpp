#include "AgentMode.h"
#include <iostream>
#include <stdio.h>
#include "../exception/NoiseStreamerException.h"

using namespace std;

AgentMode::AgentMode(
    LogService *logSrv,
    SignalAdapter* sigAdapter,
    NoiseStreamer* noiseStreamer)
    : logSrv(logSrv), sigAdapter(sigAdapter), noiseStreamer(noiseStreamer)
{
    _exit = 0;

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

void AgentMode::processCommand(string command)
{
    try
    {

        throw RuntimeException("Command not found, try 'help'");
    }
    catch (DomainException &e)
    {
        cerr << handle(e) << endl;
    }
    catch (RuntimeException &e)
    {
        cerr << handle(e) << endl;
    }
    catch (exception &e)
    {
        cerr << e.what() << endl;
    }
}
