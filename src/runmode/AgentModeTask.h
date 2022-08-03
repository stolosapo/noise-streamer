#ifndef AgentModeTask_h__
#define AgentModeTask_h__

#include <noisekernel/Thread.h>

using namespace NoiseKernel;

TaskRunner* createAgentModeTaskRunner();

void* agent_start_streamer(void* task);
void* agent_status(void* task);

#endif // AgentModeTask_h__
