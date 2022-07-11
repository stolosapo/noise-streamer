#ifndef NoiseStreamerArgument_h__
#define NoiseStreamerArgument_h__

#include <string>
#include <noisekernel/Argument.h>
#include "../logger/Logger.h"

using namespace std;
using namespace NoiseKernel;

class NoiseStreamerArgument: public ArgumentAdapter
{
private:
    static const string LOGLEVEL;
    static const string HOSTNAME;
    static const string PORT;
    static const string USERNAME;
    static const string PASSWORD;
    static const string MOUNTPOINT;
    static const string NAME;
    static const string GENRE;
    static const string DESCRIPTION;
    static const string URL;
    static const string ISPUBLIC;
    static const string BITRATE;
    static const string SAMPLERATE;
    static const string CHANNELS;

protected:
    virtual string title();

public:
    NoiseStreamerArgument(ArgumentProvider* argProvider);
    virtual ~NoiseStreamerArgument();

    virtual void registerArguments();
    bool noArgs();

    LogLevel getLogLevel();
    string getHostname();
    string getPort();
    string getUsername();
    string getPassword();
    string getMountpoint();
    string getName();
    string getGenre();
    string getDescription();
    string getUrl();
    bool getIsPublic();
    int getBitrate();
    string getSamplerate();
    string getChannels();
};

#endif // NoiseStreamerArgument_h__
