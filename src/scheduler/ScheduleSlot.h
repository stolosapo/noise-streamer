#ifndef ScheduleSlot_h__
#define ScheduleSlot_h__

#include <string>
#include "../config/PlaylistAudioSourceConfig.h"

using namespace std;

struct ScheduleSlot
{
    bool empty;
    string day;
    int start;
    int end;
    string configFile;
};

void emptySlot(ScheduleSlot& slot);
void convertSlotToPlaylistConfig(const ScheduleSlot& slot, PlaylistAudioSourceConfig& playlistConfig);

#endif // ScheduleSlot_h__
