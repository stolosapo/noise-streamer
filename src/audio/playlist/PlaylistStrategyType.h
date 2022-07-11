#ifndef PlaylistStrategyType_h__
#define PlaylistStrategyType_h__

#include <string>

using namespace std;

enum PlaylistStrategyType
{
    NONE,

    SIMPLE,

    RANDOM_ONCE
};

PlaylistStrategyType convertPlaylistStrateryType(string strategy);

#endif // PlaylistStrategyType_h__
