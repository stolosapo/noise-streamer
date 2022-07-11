#ifndef PlaylistAudioSourceItem_h__
#define PlaylistAudioSourceItem_h__

#include <string>
#include <noisekernel/Thread.h>
#include "PlaylistAudioSourceItemContext.h"
#include "../playlist/PlaylistItem.h"

using namespace std;
using namespace NoiseKernel;

class PlaylistAudioSourceItem
{
private:
    PlaylistItem track;

    bool needEncode;
    string encodedTrackFile;
    bool successEncoded;

    PlaylistAudioSourceItemContext context;

    string reencode();

    static void* encodeTrack(void* context);

public:
    PlaylistAudioSourceItem(PlaylistItem track);
    PlaylistAudioSourceItem(PlaylistItem track, PlaylistAudioSourceItemContext context);
    virtual ~PlaylistAudioSourceItem();

    PlaylistItem getTrack();
    Thread* getEncodeThread();
    PlaylistAudioSourceItemContext getContext();
    string getTrackFile() const;
    bool isSuccessEncoded();

    bool readyToPlay();
    void prepare();
    void waitToFinishEncode();
};

#endif // PlaylistAudioSourceItem_h__
