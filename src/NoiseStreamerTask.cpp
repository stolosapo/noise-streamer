#include "NoiseStreamerTask.h"
#include <noisekernel/Thread.h>

using namespace NoiseKernel;

void* noisestreamer_start(void* task)
{
    Task* t = (Task*) task;
    NoiseStreamer* ns = (NoiseStreamer*) t->getData();
    ns->connect();
    ns->stream();
    return NULL;
}

void* noisestreamer_stop(void* task)
{
    Task* t = (Task*) task;
    NoiseStreamer* ns = (NoiseStreamer*) t->getData();
    ns->shutdown();
    return NULL;
}
