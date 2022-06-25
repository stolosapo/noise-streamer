#include "LibShout.h"
#include <noisekernel/Exception.h>
#include "../exception/NoiseStreamerException.h"

using namespace NoiseKernel;

void LibShout::setProtocol(unsigned int protocol)
{
    if (shout_set_protocol(shout, protocol) != SHOUTERR_SUCCESS)
    {
        throw DomainException(NSS0017, getError());
    }
}

void LibShout::setProtocolHttp()
{
    setProtocol(SHOUT_PROTOCOL_HTTP);
}

void LibShout::setProtocolXAudioCast()
{
    setProtocol(SHOUT_PROTOCOL_XAUDIOCAST);
}

void LibShout::setProtocolIcy()
{
    setProtocol(SHOUT_PROTOCOL_ICY);
}

void LibShout::setProtocolRoarAudio()
{
    // setProtocol(SHOUT_PROTOCOL_ROARAUDIO);
}
