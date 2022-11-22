#ifndef PlaylistAudioSourceItem_h__
#define PlaylistAudioSourceItem_h__

#include <string>
#include "../playlist/PlaylistItem.h"

using namespace std;
using namespace NoiseKernel;

class PlaylistAudioSourceItem
{
private:
    PlaylistItem track;

public:
    PlaylistAudioSourceItem(PlaylistItem track);
    virtual ~PlaylistAudioSourceItem();

    PlaylistItem getTrack();
    string getTrackFile() const;
};

#endif // PlaylistAudioSourceItem_h__
