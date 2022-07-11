#include "PlaylistStrategyType.h"

PlaylistStrategyType convertPlaylistStrateryType(string strategy)
{
    if (strategy == "SIMPLE")
    {
        return SIMPLE;
    }

    if (strategy == "RANDOM_ONCE")
    {
        return RANDOM_ONCE;
    }

    return NONE;
}
