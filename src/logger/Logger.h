#ifndef NoiseStreamerLogger_h__
#define NoiseStreamerLogger_h__

#include <noisekernel/Logger.h>
#include <string>

using namespace std;

NoiseKernel::LogService buildLogService();

void fatal(string message);
void error(string message);
void warn(string message);
void info(string message);
void debug(string message);
void trace(string message);

#endif // NoiseStreamerLogger_h__
