#ifndef AgentModeTask_h__
#define AgentModeTask_h__

#include <noisekernel/Thread.h>

using namespace NoiseKernel;

template <typename T>
T* task_unwrapper(void* task);

void* agent_start_streamer(void* task);
void* agent_status(void* task);


template <typename T>
T* task_unwrapper(void* task)
{
    Task *t = (Task*) task;
    return (T*) t->getData();
}

#endif // AgentModeTask_h__
