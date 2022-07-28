#ifndef NoiseStreamerTaskRunner_h__
#define NoiseStreamerTaskRunner_h__

#include <noisekernel/Thread.h>
#include "NoiseStreamer.h"

using namespace NoiseKernel;

class NoiseStreamerTaskRunner: public TaskRunner
{
private:
    void registerTasks();

    static NoiseStreamer* task_unwrapper(void* task);
    static void* stop(void* task);

public:
    NoiseStreamerTaskRunner();
    virtual ~NoiseStreamerTaskRunner();

    string help();

    virtual void* runTask(string task, void* data);
    virtual void* runNoiseStreamerTask(string task, NoiseStreamer* noiseStreamer);
};

#endif // NoiseStreamerTaskRunner_h__
