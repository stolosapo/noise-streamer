#include "NoiseStreamer.h"

const char* NoiseStreamer::USER_AGENT = "noisestreamer";

NoiseStreamer::NoiseStreamer(
    LogService *logSrv,
    SignalAdapter* sigAdapter,
    NoiseStreamerConfig* config
): logSrv(logSrv), sigAdapter(sigAdapter), config(config)
{
}

NoiseStreamer::~NoiseStreamer()
{
}
