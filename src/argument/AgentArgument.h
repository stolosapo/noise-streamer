#ifndef AgentArgument_h__
#define AgentArgument_h__

#include <noisekernel/Argument.h>

using namespace NoiseKernel;

class AgentArgument: public ArgumentAdapter
{
private:
    static const string SERVERHOSTNAME;
    static const string SERVERPORT;
    static const string SERVERPOOLSIZE;

protected:
    virtual string title();

public:
    AgentArgument(ArgumentProvider* argProvider);
    virtual ~AgentArgument();

    virtual void registerArguments();

    string getServerHostName();
    int getServerPort();
    int getServerPoolSize();
};

#endif // AgentArgument_h__
