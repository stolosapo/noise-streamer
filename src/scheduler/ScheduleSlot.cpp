#include "ScheduleSlot.h"
#include <noisekernel/Exception.h>
#include "../config/PlaylistAudioSourceConfig.h"

using namespace NoiseKernel;

void emptySlot(ScheduleSlot& slot)
{
    slot.empty = true;
    slot.configFile = "";
    slot.day = "SUNDAY";
    slot.start = 0;
    slot.end = 0;
}

void convertSlotToPlaylistConfig(const ScheduleSlot& slot, PlaylistAudioSourceConfig& playlistConfig)
{
    if (slot.configFile == "")
    {
        throw RuntimeException("Could not find config file for slot");
    }

    buildPlaylistAudioSourceConfig(slot.configFile, playlistConfig);
}