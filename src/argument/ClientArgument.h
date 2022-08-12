#ifndef ClientArgument_h__
#define ClientArgument_h__

#include <noisekernel/Argument.h>

using namespace NoiseKernel;

class ClientArgument: public ArgumentAdapter
{
private:
    static const string AGENTHOSTNAME;
    static const string AGENTPORT;

protected:
    virtual string title();

public:
    ClientArgument(ArgumentProvider* argProvider);
    virtual ~ClientArgument();

    virtual void registerArguments();

    string getAgentHostName();
    int getAgentPort();
};

#endif // ClientArgument_h__
