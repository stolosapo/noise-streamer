#include "LibShout.h"
#include <noisekernel/Exception.h>
#include "../exception/NoiseStreamerException.h"

using namespace NoiseKernel;

void LibShout::setAgent(string agent)
{
    if (shout_set_agent(shout, agent.c_str()) != SHOUTERR_SUCCESS)
    {
        throw DomainException(NSS0004, getError());
    }
}

void LibShout::setName(string name)
{
    if (shout_set_meta(shout, SHOUT_META_NAME, name.c_str()) != SHOUTERR_SUCCESS)
    {
        throw DomainException(NSS0008, getError());
    }
}

void LibShout::setUrl(string url)
{
    if (shout_set_meta(shout, SHOUT_META_URL, url.c_str()) != SHOUTERR_SUCCESS)
    {
        throw DomainException(NSS0009, getError());
    }
}

void LibShout::setGenre(string genre)
{
    if (shout_set_meta(shout, SHOUT_META_GENRE, genre.c_str()) != SHOUTERR_SUCCESS)
    {
        throw DomainException(NSS0010, getError());
    }
}

void LibShout::setDescription(string description)
{
    if (shout_set_meta(shout, SHOUT_META_DESCRIPTION, description.c_str()) != SHOUTERR_SUCCESS)
    {
        throw DomainException(NSS0011, getError());
    }
}

void LibShout::setPublic(unsigned int make_public)
{
    if (shout_set_public(shout, make_public) != SHOUTERR_SUCCESS)
    {
        throw DomainException(NSS0015, getError());
    }
}

void LibShout::setNonblocking(unsigned int nonblocking)
{
    if (shout_set_nonblocking(shout, nonblocking) != SHOUTERR_SUCCESS)
    {
        throw DomainException(NSS0018, getError());
    }
}
