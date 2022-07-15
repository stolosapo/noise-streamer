#include "InteractiveMode.h"
#include <iostream>
#include <stdio.h>
#include "../NoiseStreamerTask.h"
#include "../exception/NoiseStreamerException.h"

using namespace std;

const char* InteractiveMode::PROMPT = "streamer> ";

InteractiveMode::InteractiveMode(
    LogService *logSrv,
    SignalAdapter* sigAdapter,
    NoiseStreamer* noiseStreamer)
    : TaskRunner(), logSrv(logSrv), sigAdapter(sigAdapter), noiseStreamer(noiseStreamer)
{
    registerTasks();
    _exit = 0;

    th = new Thread;
    th->attachDelegate(&InteractiveMode::noiseStreamerThreadDelegate);
}

InteractiveMode::~InteractiveMode()
{
    if (th != NULL)
    {
        delete th;
    }
}

void* InteractiveMode::noiseStreamerThreadDelegate(void* data)
{
    NoiseStreamer* ns = (NoiseStreamer*) data;
    ns->initialize();
    ns->connect();
    ns->stream();
    ns->disconnect();
    ns->shutdown();
    return NULL;
}

void* InteractiveMode::help(void*)
{
    return static_cast<void*>(new string("Some help.."));
}

void InteractiveMode::registerTasks()
{
    // registerTask("start", &noisestreamer_start);
    registerTask("stop", &noisestreamer_stop);
    registerTask("exit", &noisestreamer_stop);
    registerTask("help", &help);
}

void InteractiveMode::processCommand(string command)
{
    if (command == "exit")
    {
        _exit = 1;
    }

    try
    {
        if (!taskExist(command))
        {
            cerr << "Command not found, try 'help'" << endl;
            return;
        }

        void* result = runTask(command, noiseStreamer);
        if (result != NULL)
    	{
    		string *str = static_cast<string*>(result);
            string strValue = *str;

            cout << strValue << endl;

    		delete str;
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
    // Run streamer in different thread
    th->start(noiseStreamer);

    while (_exit == 0 && !sigAdapter->gotSigInt())
    {
        cout << PROMPT;
        string command;
	    getline(cin , command);

        processCommand(command);
    }

    th->wait();
}
