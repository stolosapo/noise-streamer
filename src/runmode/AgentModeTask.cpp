#include "AgentModeTask.h"
#include "AgentMode.h"
#include "../NoiseStreamerTask.h"
#include "../exception/NoiseStreamerException.h"
#include "../utils/StringHelper.h"
#include "../utils/TaskHelper.h"

TaskRunner* createAgentModeTaskRunner()
{
    TaskRunner* runner = new TaskRunner;
    runner->registerTask("start", &agent_start_streamer);
    runner->registerTask("agent-status", &agent_status);
    return runner;
}

string createAgentModeTaskRunnerHelp()
{
    string result = "\n"
    "Agent :: Tasks\n"
    "==============\n"
    "\n"
    "start          Starts the NoiseStreamer\n"
    "agent-status   See the status of the Agent and NoiseStreamer"
    "\n" +
    createNoiseStreamerTaskRunnerHelp();

    return result;
}

void* agent_start_streamer(void* task)
{
    AgentMode* self = unwrap_task<AgentMode>(task);

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
    AgentMode* self = unwrap_task<AgentMode>(task);

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
