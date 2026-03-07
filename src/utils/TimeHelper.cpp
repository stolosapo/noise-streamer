#include "TimeHelper.h"

#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <exception>
#include <noisekernel/Exception.h>
#include "StringHelper.h"

using namespace NoiseKernel;

const char* days[] = {
    "SUNDAY",
    "MONDAY",
    "TUESDAY",
    "WEDNESDAY",
    "THURSDAY",
    "FRIDAY",
    "SATURDAY"
};

Locker _localTimeLocker = initLocalTimeLocker();

Locker initLocalTimeLocker()
{
    Locker lock = Locker();
    lock.init();
    return lock;
}

void getCurrentSystemTime(string& day, int& minutes)
{
    _localTimeLocker.lock();

    try
    {
        time_t t = time(0);
        struct  tm* now = localtime(&t);

        if (now == NULL)
        {
            throw RuntimeException("Failed to obtain local time in getCurrentSystemTime.");
        }

        day = days[now->tm_wday];
        minutes = (now->tm_hour * 60) + now->tm_min;
    }
    catch (exception &e)
    {
        cerr << e.what() << endl;
    }

    _localTimeLocker.unlock();
}

int timeToMinutes(string time)
{
    int hours = 0, minutes = 0;
    char colon = ':';

    stringstream ss(time);
    ss >> hours >> colon >> minutes;
    return (hours * 60) + minutes;
}
