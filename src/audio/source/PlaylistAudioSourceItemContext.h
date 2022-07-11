#ifndef PlaylistAudioSourceItemContext_h__
#define PlaylistAudioSourceItemContext_h__

#include <string>
#include <noisekernel/Thread.h>
#include <noisekernel/Logger.h>
#include "../playlist/PlaylistItem.h"

using namespace std;
using namespace NoiseKernel;

struct PlaylistAudioSourceItemContext
{
    Thread* encodeThread;
    // AudioEncodingService* encSrv;
    LogService* logSrv;

    string pcmOutPath;
    string mp3OutPath;

    AudioEncodeMode encodeMode;
    AudioBitrate audioBitrate;
    int samplerate;
    int quality;
};

#endif // PlaylistAudioSourceItemContext_h__
