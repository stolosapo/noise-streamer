#include "NoiseStreamerTask.h"

void* noisestreamer_start(void* noiseStreamer)
{
    NoiseStreamer* ns = (NoiseStreamer*) noiseStreamer;
    ns->connect();
    ns->stream();
    return NULL;
}

void* noisestreamer_stop(void* noiseStreamer)
{
    NoiseStreamer* ns = (NoiseStreamer*) noiseStreamer;
    ns->disconnect();
    ns->shutdown();
    return NULL;
}
