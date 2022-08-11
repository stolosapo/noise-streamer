#include "NoiseStreamerTask.h"
#include "audio/source/PlaylistAudioSourceTask.h"
#include "utils/TaskHelper.h"
#include "utils/StringHelper.h"

TaskRunner* createNoiseStreamerTaskRunner()
{
    TaskRunner* runner = new TaskRunner;
    runner->registerTask("stop", &noisestreamer_stop);

    runner->registerTask("playlist::audio-status", &noisestreamer_playlist_task);
    runner->registerTask("playlist::now", &noisestreamer_playlist_task);
    runner->registerTask("playlist::preview", &noisestreamer_playlist_task);
    runner->registerTask("playlist::preview-track", &noisestreamer_playlist_task);
    runner->registerTask("playlist::history", &noisestreamer_playlist_task);
    runner->registerTask("playlist::next", &noisestreamer_playlist_task);
    runner->registerTask("playlist::request", &noisestreamer_playlist_task);

    return runner;
}

string createNoiseStreamerTaskRunnerHelp()
{
    string result = "\n"
    "NoiseStreamer :: Tasks\n"
    "======================\n"
    "\n"
    "stop        Stops the NoiseStreamer\n"
    "\n" +
    createPlaylistAudioSourceTaskRunnerHelp();

    return result;
}

void* noisestreamer_stop(void* task)
{
    NoiseStreamer* ns = unwrap_task<NoiseStreamer>(task);
    ns->stop();
    return static_cast<void*>(new string("NoiseStreamer stopped streaming!"));
}

void* noisestreamer_playlist_task(void* task)
{
    Task* t = (Task*) task;
    NoiseStreamer* ns = (NoiseStreamer*) t->getData();
    string taskFullName = build_full_task_name(t);
    return ns->getAudioSource()->runCommand(taskFullName);
}
