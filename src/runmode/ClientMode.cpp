#include "ClientMode.h"

ClientMode::ClientMode(
    LogService *logSrv,
    SignalAdapter *sigSrv,
    TcpClientConfig *config,
    TcpProtocol *protocol)
    : TcpClient(logSrv, sigSrv, config, protocol)
{

}

ClientMode::~ClientMode()
{

}
