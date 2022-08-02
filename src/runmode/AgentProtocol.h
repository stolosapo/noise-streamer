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

    static AgentProtocol* createForServer();
    static AgentProtocol* createForClient();

    virtual string prompt();
};

#endif // AgentProtocol_h__
