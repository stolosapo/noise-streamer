#ifndef PlaylistAudioSourceArgument_h__
#define PlaylistAudioSourceArgument_h__

#include <string>
#include <noisekernel/Argument.h>

using namespace std;
using namespace NoiseKernel;

class PlaylistAudioSourceArgument: public ArgumentAdapter
{
private:
    static const string PLAYLISTFILEPATH;
    static const string HISTORYFILEPATH;
    static const string STRATEGYTYPE;
    static const string REPEAT;
    static const string REENCODE;
    static const string PCMOUTPATH;
    static const string MP3OUTPATH;

protected:
    virtual string title();

public:
    PlaylistAudioSourceArgument(ArgumentProvider* argProvider);
    virtual ~PlaylistAudioSourceArgument();

    virtual void registerArguments();

    string getPlaylistFilePath();
    string getHistoryFilePath();
    string getStrategyType();
    bool getRepeat();
    bool getReencode();
    string getPcmOutPath();
    string getMp3OutPath();
};

#endif // PlaylistAudioSourceArgument_h__
