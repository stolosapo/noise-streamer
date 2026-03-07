#ifndef Scheduler_h__
#define Scheduler_h__

#include <string>
#include <vector>
#include <noisekernel/Logger.h>
#include <noisekernel/Signal.h>
#include <noisekernel/Thread.h>
#include <noisekernel/Observer.h>

#include "ScheduleSlot.h"

using namespace std;
using namespace NoiseKernel;

class Scheduler
{
private:
    volatile sig_atomic_t _stop;

    LogService* logSrv;
    SignalAdapter* sigAdapter;

    Thread* schedulerThread;
    Locker _locker;
    vector<ScheduleSlot> slots;
    ScheduleSlot lastSlot;

    static void* checkSchedule(void* scheduler);
    void findCurrentSlot();

public:
    Event SlotChanged;

    Scheduler(
        LogService *logSrv,
        SignalAdapter* sigAdapter);
    virtual ~Scheduler();

    void load(string scheduleFilePath);
    void start();
    void stop();
    ScheduleSlot currentSlot();
};

#endif // Scheduler_h__
