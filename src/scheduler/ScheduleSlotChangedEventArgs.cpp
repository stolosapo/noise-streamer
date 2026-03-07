#include "ScheduleSlotChangedEventArgs.h"

ScheduleSlotChangedEventArgs::ScheduleSlotChangedEventArgs(ScheduleSlot slot)
    : slot(slot)
{

}

ScheduleSlotChangedEventArgs::~ScheduleSlotChangedEventArgs()
{

}

ScheduleSlot ScheduleSlotChangedEventArgs::getSlot()
{
    return slot;
}
