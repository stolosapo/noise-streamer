#ifndef NoiseStreamerConfig_h__
#define NoiseStreamerConfig_h__

#include <string>
#include "../audio/AudioBitrate.h"

using namespace std;

struct NoiseStreamerConfig
{
    string hostname;
    string port;
    string username;
    string password;
    string protocol;
    string mountpoint;
    string name;
    string genre;
    string description;
    string url;
    string isPublic;
    string bitrate;
    AudioBitrate typedBitrate;
    string samplerate;
    string channels;
};

#endif // NoiseStreamerConfig_h__