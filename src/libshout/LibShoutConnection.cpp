#include "LibShout.h"
#include "../utils/StringHelper.h"
#include <noisekernel/Exception.h>
#include "../exception/NoiseStreamerException.h"

using namespace NoiseKernel;

int LibShout::shoutOpen()
{
    return shout_open(shout);
}

int LibShout::shoutClose()
{
    return shout_close(shout);
}

/* returns SHOUTERR_CONNECTED or SHOUTERR_UNCONNECTED */
int LibShout::getConnected()
{
    return shout_get_connected(shout);
}

bool LibShout::isConnected()
{
    return getConnected() == SHOUTERR_CONNECTED;
}

void LibShout::setHost(string host)
{
    if (shout_set_host(shout, host.c_str()) != SHOUTERR_SUCCESS)
    {
        throw DomainException(NSS0002, getError());
    }
}

string LibShout::getHost()
{
    return string(shout_get_host(shout));
}

void LibShout::setPort(unsigned short port)
{
    if (shout_set_port(shout, port) != SHOUTERR_SUCCESS)
    {
        throw DomainException(NSS0003, getError());
    }
}

string LibShout::getPort()
{
    return numberToString<unsigned short>(shout_get_port(shout));
}

void LibShout::setUser(string username)
{
    if (shout_set_user(shout, username.c_str()) != SHOUTERR_SUCCESS)
    {
        throw DomainException(NSS0005, getError());
    }
}

void LibShout::setPassword(string password)
{
    if (shout_set_password(shout, password.c_str()) != SHOUTERR_SUCCESS)
    {
        throw DomainException(NSS0006, getError());
    }
}

void LibShout::setMount(string mount)
{
    if (shout_set_mount(shout, mount.c_str()) != SHOUTERR_SUCCESS)
    {
        throw DomainException(NSS0007, getError());
    }
}

string LibShout::getMount()
{
    return string(shout_get_mount(shout));
}
