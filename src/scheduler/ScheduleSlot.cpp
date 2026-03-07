#include "ScheduleSlot.h"

void emptySlot(ScheduleSlot& slot)
{
    slot.empty = true;
    slot.configFile = "";
    slot.day = "SUNDAY";
    slot.start = 0;
    slot.end = 0;
}
