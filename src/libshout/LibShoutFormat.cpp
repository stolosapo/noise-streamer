#include "LibShout.h"
#include <noisekernel/Exception.h>
#include "../exception/NoiseStreamerException.h"

using namespace NoiseKernel;

void LibShout::setFormat(unsigned int format)
{
    if (shout_set_content_format(shout, format, SHOUT_USAGE_AUDIO, NULL) != SHOUTERR_SUCCESS)
    {
        throw DomainException(NSS0016, getError());
    }
}

void LibShout::setFormatOgg()
{
    setFormat(SHOUT_FORMAT_OGG);
}

void LibShout::setFormatMp3()
{
    setFormat(SHOUT_FORMAT_MP3);
}

void LibShout::setFormatWebM()
{
    setFormat(SHOUT_FORMAT_WEBM);
}

void LibShout::setFormatWebMAudio()
{
    setFormat(SHOUT_FORMAT_WEBMAUDIO);
}
