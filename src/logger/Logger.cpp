#include "Logger.h"
#include <iostream>

using namespace std;

const LogLevel defaultLogLevel = INFO;
LogLevel logLevel = defaultLogLevel;

LogLevel convertLogLevelFromString(string logLevel)
{
    if (logLevel == "TRACE")
    {
        return TRACE;
    }

    if (logLevel == "DEBUG")
    {
        return DEBUG;
    }

    if (logLevel == "INFO")
    {
        return INFO;
    }

    if (logLevel == "WARN")
    {
        return WARN;
    }

    if (logLevel == "ERROR")
    {
        return ERROR;
    }

    if (logLevel == "FATAL")
    {
        return FATAL;
    }

    return defaultLogLevel;
}

string convertLogLevelToString(LogLevel logLevel)
{
    switch (logLevel)
    {
    case TRACE:
        return "TRACE";

    case DEBUG:
        return "DEBUG";

    case INFO:
        return "INFO";

    case WARN:
        return "WARN";

    case ERROR:
        return "ERROR";

    case FATAL:
        return "FATAL";

    default:
        return "";
    }
}

NoiseKernel::LogService buildLogService()
{
    NoiseKernel::LogService logger;
    logger.fatal = &fatal;
    logger.error = &error;
    logger.warn = &warn;
    logger.info = &info;
    logger.debug = &debug;
    logger.trace = &trace;

    return logger;
}

void fatal(string message)
{
    cout << "[FATAL] " << time(0) << " " << message << endl;
}

void error(string message)
{
    if (logLevel > ERROR)
    {
        return;
    }

    cout << "[ERROR] "  << time(0) << " " << message << endl;
}

void warn(string message)
{
    if (logLevel > WARN)
    {
        return;
    }

    cout << "[WARN] "  << time(0) << " " << message << endl;
}

void info(string message)
{
    if (logLevel > INFO)
    {
        return;
    }

    cout << "[INFO] "  << time(0) << " " << message << endl;
}

void debug(string message)
{
    if (logLevel > DEBUG)
    {
        return;
    }

    cout << "[DEBUG] "  << time(0) << " " << message << endl;
}

void trace(string message)
{
    if (logLevel > TRACE)
    {
        return;
    }

    cout << "[TRACE] "  << time(0) << " " << message << endl;
}
