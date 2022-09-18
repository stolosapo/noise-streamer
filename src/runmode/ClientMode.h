#ifndef ClientMode_h__
#define ClientMode_h__

#include <string>
#include <noisekernel/Tcp.h>

using namespace std;
using namespace NoiseKernel;

class ClientMode: public TcpClient
{
public:
    ClientMode(
        LogService *logSrv,
        SignalAdapter *sigSrv,
        TcpClientConfig *config,
        TcpProtocol *protocol);
    virtual ~ClientMode();
};

#endif // ClientMode_h__
