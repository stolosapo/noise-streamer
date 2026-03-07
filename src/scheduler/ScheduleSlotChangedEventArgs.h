#ifndef ScheduleSlotChangedEventArgs_h__
#define ScheduleSlotChangedEventArgs_h__

#include <string>
#include <noisekernel/Observer.h>
#include "ScheduleSlot.h"

using namespace std;
using namespace NoiseKernel;

class ScheduleSlotChangedEventArgs: public EventArgs
{
private:
    ScheduleSlot slot;

public:
    ScheduleSlotChangedEventArgs(ScheduleSlot slot);
    virtual ~ScheduleSlotChangedEventArgs();

    ScheduleSlot getSlot();
};

#endif // ScheduleSlotChangedEventArgs_h__
