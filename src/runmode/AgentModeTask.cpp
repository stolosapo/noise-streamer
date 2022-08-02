#include "AgentModeTask.h"
#include "AgentMode.h"
#include "../exception/NoiseStreamerException.h"
#include "../utils/StringHelper.h"

void* agent_start_streamer(void* task)
{
    AgentMode* self = task_unwrapper<AgentMode>(task);

    if (self->th != NULL && self->th->isRunning())
    {
        throw RuntimeException("NoiseStreamer is already running!");
    }

    if (self->th != NULL)
    {
        self->th->wait();
        delete self->th;
    }

    self->th = self->noiseStreamer->startAsync();
    return static_cast<void*>(new string("NoiseStreamer started streaming!"));
}

void* agent_status(void* task)
{
    AgentMode* self = task_unwrapper<AgentMode>(task);

    double uptimeSec = self->uptime();
    int connections = self->numberOfActiveConnections();

    int sec = uptimeSec;

    int days = sec / 60 / 60 / 24;
    int hours = (sec / 60 / 60) % 24;
    int minutes = (sec / 60) % 60;
    int seconds = sec % 60;

    char s[25];
    sprintf(s, "%01d days, %02d:%02d:%02d", days, hours, minutes, seconds);
    string str(s);

    string status = "not running";
    if (self->th->isRunning())
    {
        status = "running";
    }

    string value = "\n";

    value += "User-Agent: " + string(self->noiseStreamer->userAgent()) + "\n";
    value += "NoiseStreamer Status: " + status + "\n";
    value += "Uptime: " + str + "\n";
    value += "Active connections: " + numberToString<int>(connections) + "\n";

    return static_cast<void*>(new string(value));
}
