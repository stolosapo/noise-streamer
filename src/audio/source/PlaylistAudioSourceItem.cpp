#include "PlaylistAudioSourceItem.h"

#include "../../exception/NoiseStreamerException.h"
#include "../../utils/StringHelper.h"
#include "../../utils/FileHelper.h"

PlaylistAudioSourceItem::PlaylistAudioSourceItem(PlaylistItem track)
    : track(track)
{
}

PlaylistAudioSourceItem::~PlaylistAudioSourceItem()
{

}

PlaylistItem PlaylistAudioSourceItem::getTrack()
{
    return track;
}

string PlaylistAudioSourceItem::getTrackFile() const
{
    return track.getTrack();
}
