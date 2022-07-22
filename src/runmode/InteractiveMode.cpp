#include "InteractiveMode.h"
#include <iostream>
#include <stdio.h>
#include "../NoiseStreamerTask.h"
#include "../exception/NoiseStreamerException.h"

using namespace std;

const char* InteractiveMode::PROMPT = "streamer> ";
const char* InteractiveMode::START = "start";
const char* InteractiveMode::STOP = "stop";
const char* InteractiveMode::EXIT = "exit";
const char* InteractiveMode::HELP = "help";

InteractiveMode::InteractiveMode(
    LogService *logSrv,
    SignalAdapter* sigAdapter,
    NoiseStreamer* noiseStreamer)
    : logSrv(logSrv), sigAdapter(sigAdapter), noiseStreamer(noiseStreamer)
{
    _exit = 0;

    th = NULL;
}

InteractiveMode::~InteractiveMode()
{
    if (th != NULL)
    {
        delete th;
    }
}

void InteractiveMode::startNoiseStreamerAsync()
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

void InteractiveMode::stopNoiseStreamer()
{
    if (th != NULL && !th->isRunning())
    {
        throw RuntimeException("NoiseStreamer is already stopped!");
    }

    noiseStreamer->stop();
    cout << "NoiseStreamer stopped streaming!" << endl;
}

string InteractiveMode::help()
{
    return "Some help..";
}

void InteractiveMode::processCommand(string command)
{
    try
    {
        if (command == START)
        {
            startNoiseStreamerAsync();
        }
        else if (command == STOP)
        {
            stopNoiseStreamer();
        }
        else if (command == EXIT)
        {
            noiseStreamer->stop();
            _exit = 1;
        }
        else if (command == HELP)
        {
            cout << help() << endl;
        }
        else
        {
            throw RuntimeException("Command not found, try 'help'");
        }
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

void InteractiveMode::start()
{
    startNoiseStreamerAsync();

    while (_exit == 0 && !sigAdapter->gotSigInt())
    {
        cout << PROMPT;
        string command;
	    getline(cin , command);

        processCommand(command);
    }

    th->wait();
}
