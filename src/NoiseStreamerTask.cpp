#include "NoiseStreamerTask.h"
#include "utils/TaskHelper.h"

TaskRunner* createNoiseStreamerTaskRunner()
{
    TaskRunner* runner = new TaskRunner;
    runner->registerTask("stop", &noisestreamer_stop);
    return runner;
}

void* noisestreamer_stop(void* task)
{
    NoiseStreamer* ns = unwrap_task<NoiseStreamer>(task);
    ns->stop();
    return static_cast<void*>(new string("NoiseStreamer stopped streaming!"));
}
