#include "Scheduler.h"

#include <unistd.h>
#include <sstream>
#include "../exception/NoiseStreamerException.h"
#include "../utils/File.h"
#include "../utils/TimeHelper.h"
#include "../utils/StringHelper.h"
#include "ScheduleSlotChangedEventArgs.h"

using namespace std;

Scheduler::Scheduler(
    LogService *logSrv,
    SignalAdapter* sigAdapter
) : logSrv(logSrv), 
    sigAdapter(sigAdapter),
    schedulerThread(NULL)
{
    _locker.init();
    _stop = 0;
    emptySlot(lastSlot);
}

Scheduler::~Scheduler()
{
    _stop = 1;

    if (schedulerThread != NULL)
    {
        schedulerThread->wait();
        delete schedulerThread;
    }

    _locker.destroy();
}

void Scheduler::load(string scheduleFilePath)
{
    struct LockerGuard
    {
        decltype(_locker) &locker;
        
        explicit LockerGuard(decltype(_locker) &l)
            : locker(l)
        {
            locker.lock();
        }

        ~LockerGuard()
        {
            locker.unlock();
        }
    } guard(_locker);

    slots.clear();

    File file(scheduleFilePath);
    file.load();

    for (int i = 0; i < file.size(); i++)
    {
        string line = file.read(i);
        if (line.empty())
        {
            continue;
        }

        stringstream ss(line);
        string day, start, end, configFile;

        getline(ss, day, ',');
        getline(ss, start, ',');
        getline(ss, end, ',');
        getline(ss, configFile, ',');

        ScheduleSlot slot;
        slot.day = day;
        slot.start = timeToMinutes(start);
        slot.end = timeToMinutes(end);
        slot.configFile = configFile;
        slot.empty = false;

        slots.push_back(slot);
    }

    logSrv->info("Scheduler loaded with '" + numberToString<int>(slots.size()) + "' slots");
}

void Scheduler::start()
{
    if (schedulerThread != NULL)
    {
        logSrv->warn("Scheduler thread already is running, skipping..");
        return;
    }
    
    _stop = 0;
    
    schedulerThread = new Thread();
    schedulerThread->attachDelegate(&Scheduler::checkSchedule);
    schedulerThread->start(this);
}

void Scheduler::stop()
{
    if (schedulerThread == NULL)
    {
        logSrv->warn("Scheduler thread is already stopped, skipping..");
        return;
    }

    _stop = 1;

    schedulerThread->wait();
    delete schedulerThread;
    schedulerThread = NULL;
}

void* Scheduler::checkSchedule(void* scheduler)
{
    Scheduler* self = (Scheduler*) scheduler;

    try
    {
        while (!self->sigAdapter->gotSigInt() && self->_stop == 0)
        {
            self->findCurrentSlot();
            sleep(1);
        }
    }
    catch(DomainException& e)
    {
        self->_locker.unlock();
        self->logSrv->error(handle(e));
    }
    catch(RuntimeException& e)
    {
        self->_locker.unlock();
        self->logSrv->error(handle(e));
    }
    catch(exception& e)
    {
        self->_locker.unlock();
        self->logSrv->error(e.what());
    }

    self->logSrv->info("Scheduler stopped checking!");
    
    return NULL;
}

void Scheduler::findCurrentSlot()
{
    _locker.lock();

    string currentDay;
    int currentMinutes;
    getCurrentSystemTime(currentDay, currentMinutes);

    ScheduleSlot currentSlot;
    emptySlot(currentSlot);

    for (size_t i = 0; i < slots.size(); i++)
    {
        ScheduleSlot slot = slots[i];
        if (slot.day == currentDay && 
            currentMinutes >= slot.start &&
            currentMinutes < slot.end)
        {
            currentSlot = slot;
            break;
        }
    }

    bool changed = false;
    if (currentSlot.empty != lastSlot.empty ||
        currentSlot.configFile != lastSlot.configFile)
    {
        lastSlot = currentSlot;
        changed = true;
    }

    _locker.unlock();

    if (changed)
    {
        logSrv->debug("Slot changed to program: '" + currentSlot.configFile + "'");
        ScheduleSlotChangedEventArgs* arg = new ScheduleSlotChangedEventArgs(currentSlot);
        SlotChanged.raise(this, arg);
    }
}

ScheduleSlot Scheduler::currentSlot()
{
    _locker.lock();
    ScheduleSlot slot = lastSlot;
    _locker.unlock();
    return slot;
}
