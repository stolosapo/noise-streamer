#ifndef NoiseStreamerTaskRunner_h__
#define NoiseStreamerTaskRunner_h__

#include <noisekernel/Thread.h>
#include "NoiseStreamer.h"

using namespace NoiseKernel;

TaskRunner* createNoiseStreamerTaskRunner();
string createNoiseStreamerTaskRunnerHelp();

void* noisestreamer_stop(void* task);
void* noisestreamer_playlist_task(void* task);

#endif // NoiseStreamerTaskRunner_h__
