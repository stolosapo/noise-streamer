#ifndef AgentArgument_h__
#define AgentArgument_h__

#include <noisekernel/Argument.h>

using namespace NoiseKernel;

class AgentArgument: public ArgumentAdapter
{
private:
    static const string AGENTHOSTNAME;
    static const string AGENTPORT;
    static const string AGENTPOOLSIZE;

protected:
    virtual string title();

public:
    AgentArgument(ArgumentProvider* argProvider);
    virtual ~AgentArgument();

    virtual void registerArguments();

    string getAgentHostName();
    int getAgentPort();
    int getAgentPoolSize();
};

#endif // AgentArgument_h__
