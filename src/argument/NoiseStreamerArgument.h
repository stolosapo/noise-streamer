#ifndef NoiseStreamerArgument_h__
#define NoiseStreamerArgument_h__

#include <string>
#include <noisekernel/Argument.h>
#include "../logger/Logger.h"
#include "../runmode/RunMode.h"

using namespace std;
using namespace NoiseKernel;

class NoiseStreamerArgument: public ArgumentAdapter
{
private:
    static const string RUNMODE;
    static const string LOGLEVEL;
    static const string LOGFILE;
    static const string BACKGROUND;
    static const string PIDFILE;
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

    RunMode getRunMode();
    LogLevel getLogLevel();
    bool shouldLogToFile();
    string logFile();
    bool runOnBackground();
    string pidFile();
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
