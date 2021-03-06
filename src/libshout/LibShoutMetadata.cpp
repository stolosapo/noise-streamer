#include "LibShout.h"
#include <noisekernel/Exception.h>
#include "../exception/NoiseStreamerException.h"

using namespace NoiseKernel;

shout_metadata_t* LibShout::createNewMetadata()
{
    shout_metadata_t* newMetadata;

    if (!(newMetadata = shout_metadata_new()))
    {
        throw DomainException(NSS0014, getError());
    }

    return newMetadata;
}

void LibShout::setMeta(shout_metadata_t* newMetadata)
{
    if (!successLastAction())
    {
        return;
    }

    int m = shout_set_metadata_utf8(shout, newMetadata);
    if (m != SHOUTERR_SUCCESS)
    {
        logSrv->warn("Updating metadata failed. " + string(getError()));
    }
}

void LibShout::addMeta(shout_metadata_t* metadata, string name, string value)
{
    if (shout_metadata_add(metadata, name.c_str(), value.c_str()) != SHOUTERR_SUCCESS)
    {
        throw DomainException(NSS0014, getError());
    }
}

void LibShout::addMetaSong(shout_metadata_t* metadata, string song)
{
    addMeta(metadata, "song", song);
}

void LibShout::freeMetadata(shout_metadata_t* metadata)
{
    shout_metadata_free(metadata);
}

void LibShout::updateMetadata(string metadata)
{
    shout_metadata_t* newMetadata = createNewMetadata();
    addMetaSong(newMetadata, metadata);
    setMeta(newMetadata);
    freeMetadata(newMetadata);
}
