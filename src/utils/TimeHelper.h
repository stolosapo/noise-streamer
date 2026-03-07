#ifndef TimeHelper_h__
#define TimeHelper_h__

#include <iostream>
#include <string>
#include <sstream>
#include <ctime>

#include <noisekernel/Thread.h>

using namespace std;
using namespace NoiseKernel;

extern const char* days[];

extern Locker _localTimeLocker;

Locker initLocalTimeLocker();
void getCurrentSystemTime(string& day, int& minutes);
int timeToMinutes(string time);

#endif // TimeHelper_h__
