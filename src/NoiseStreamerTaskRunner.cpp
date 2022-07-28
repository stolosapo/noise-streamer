#include "NoiseStreamerTaskRunner.h"

NoiseStreamerTaskRunner::NoiseStreamerTaskRunner(): TaskRunner()
{
    registerTasks();
}

NoiseStreamerTaskRunner::~NoiseStreamerTaskRunner()
{

}

string NoiseStreamerTaskRunner::help()
{
    return "Some help..";
}

void* NoiseStreamerTaskRunner::runTask(string task, void* data)
{
    return TaskRunner::runTask(task, data);
}

void* NoiseStreamerTaskRunner::runNoiseStreamerTask(string task, NoiseStreamer* noiseStreamer)
{
    return runTask(task, noiseStreamer);
}

void NoiseStreamerTaskRunner::registerTasks()
{
    registerTask("stop", &stop);
}

NoiseStreamer* NoiseStreamerTaskRunner::task_unwrapper(void* task)
{
    Task *t = (Task*) task;
    return (NoiseStreamer*) t->getData();
}

void* NoiseStreamerTaskRunner::stop(void* task)
{
    NoiseStreamer* ns = task_unwrapper(task);
    ns->stop();
    return static_cast<void*>(new string("NoiseStreamer stopped streaming!"));
}
