#ifndef AgentProtocol_h__
#define AgentProtocol_h__

#include <string>
#include <noisekernel/Tcp.h>

using namespace std;
using namespace NoiseKernel;

class AgentProtocol: public TcpProtocol
{
public:
    AgentProtocol(bool isServer);
    virtual ~AgentProtocol();

    virtual string help();
    virtual string prompt();
};

#endif // AgentProtocol_h__
