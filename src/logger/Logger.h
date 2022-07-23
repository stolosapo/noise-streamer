#ifndef NoiseStreamerLogger_h__
#define NoiseStreamerLogger_h__

#include <noisekernel/Logger.h>
#include <string>
#include <time.h>

using namespace std;

enum LogLevel
{
    TRACE = 0,

    DEBUG = 1,

    INFO = 2,

    WARN = 3,

    ERROR = 4,

    FATAL = 5
};

extern const LogLevel defaultLogLevel;
extern LogLevel logLevel;
extern NoiseKernel::LogService rootLogService;
extern bool shouldLogOnFile;
extern string logFile;

LogLevel convertLogLevelFromString(string logLevel);
string convertLogLevelToString(LogLevel logLevel);
string formatLogEntry(LogLevel logLevel, time_t time, string message);

NoiseKernel::LogService buildLogService();

void fatal(string message);
void error(string message);
void warn(string message);
void info(string message);
void debug(string message);
void trace(string message);

#endif // NoiseStreamerLogger_h__
