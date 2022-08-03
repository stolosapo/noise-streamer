#ifndef TaskHelper_h__
#define TaskHelper_h__

#include <noisekernel/Thread.h>

using namespace NoiseKernel;

template <typename T>
T* unwrap_task(void* task);

template <typename T>
T* unwrap_task(void* task)
{
    Task *t = (Task*) task;
    return (T*) t->getData();
}

#endif // TaskHelper_h__
