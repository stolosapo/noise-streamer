#include <unistd.h>
#include <fstream>
#include <noisekernel/Exception.h>

#include "LibShout.h"
#include "../exception/NoiseStreamerException.h"
#include "../utils/StringHelper.h"

using namespace NoiseKernel;

LibShout::LibShout(
    LogService *logSrv,
    SignalAdapter* sigAdapter
): logSrv(logSrv), sigAdapter(sigAdapter)
{
}

LibShout::~LibShout()
{
}

void LibShout::finilizeShout()
{
    shoutClose();

    shoutShutdown();

    shoutFree();
}

void LibShout::initializeShout()
{
    shoutInit();

    shoutNew();
}

void LibShout::startShout()
{
    string server_url = "http://" + getHost() + ":" + getPort() + getMount();

    long ret = shoutOpen();
    if (ret == SHOUTERR_SUCCESS)
    {
        ret = SHOUTERR_CONNECTED;
    }

    if (ret == SHOUTERR_BUSY)
    {
        logSrv->info("Connection to [" + server_url + "] is pending...");
    }

    while (ret == SHOUTERR_BUSY && !sigAdapter->gotSigInt())
    {
        usleep(10000);
        ret = getConnected();
    }

    if (ret != SHOUTERR_CONNECTED)
    {
        DomainException err(NSS0019, getError());
        logSrv->warn(handle(err));
        restartShout();
    }
    else
    {
        logSrv->info("Connected to server: " + server_url);
    }
}

void LibShout::restartShout()
{
    if (isConnected())
    {
        return;
    }

    while (!isConnected() && !sigAdapter->gotSigInt())
    {
        startShout();
    }
}

bool LibShout::successLastAction()
{
    int errNo = getErrno();

    /*
    SHOUTERR_SUCCESS		(0) /* No error
    SHOUTERR_INSANE			(-1) /* Nonsensical arguments e.g. self being NULL
    SHOUTERR_NOCONNECT		(-2) /* Couldn't connect
    SHOUTERR_NOLOGIN		(-3) /* Login failed
    SHOUTERR_SOCKET			(-4) /* Socket error
    SHOUTERR_MALLOC			(-5) /* Out of memory
    SHOUTERR_METADATA		(-6)
    SHOUTERR_CONNECTED		(-7) /* Cannot set parameter while connected
    SHOUTERR_UNCONNECTED		(-8) /* Not connected
    SHOUTERR_UNSUPPORTED		(-9) /* This libshout doesn't support the requested option
    SHOUTERR_BUSY			(-10) /* Socket is busy
    SHOUTERR_NOTLS                  (-11) /* TLS requested but not supported by peer
    SHOUTERR_TLSBADCERT             (-12) /* TLS connection can not be established because of bad certificate
    SHOUTERR_RETRY                  (-13) /* Retry last operation.
    */

    return errNo == SHOUTERR_SUCCESS;
}
