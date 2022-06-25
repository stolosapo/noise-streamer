#include "NoiseStreamer.h"

const char* NoiseStreamer::USER_AGENT = "noisestreamer";

NoiseStreamer::NoiseStreamer(
    LogService *logSrv,
    SignalAdapter* sigAdapter
): logSrv(logSrv), sigAdapter(sigAdapter)
{
}

NoiseStreamer::~NoiseStreamer()
{
}
