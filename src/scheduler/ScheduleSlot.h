#ifndef ScheduleSlot_h__
#define ScheduleSlot_h__

#include <string>

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

#endif // ScheduleSlot_h__
