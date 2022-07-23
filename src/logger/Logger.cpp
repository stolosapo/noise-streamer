#include "Logger.h"
#include <iostream>
#include "../utils/StringHelper.h"
#include "../utils/FileHelper.h"

using namespace std;

const LogLevel defaultLogLevel = INFO;
LogLevel logLevel = defaultLogLevel;
NoiseKernel::LogService rootLogService = buildLogService();
bool shouldLogOnFile = false;
string logFile = "";

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

string formatLogEntry(LogLevel logLevel, time_t time, string message)
{
    return "[" + convertLogLevelToString(logLevel) + "] " +
    numberToString<time_t>(time) + " " +
    message;
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
    string logEntry = formatLogEntry(FATAL, time(0), message);
    if (shouldLogOnFile)
    {
        appendLineFileToFile(logFile.c_str(), logEntry);
    }
    else
    {
        cout << logEntry << endl;
    }
    // cout << "[FATAL] " << time(0) << " " << message << endl;
}

void error(string message)
{
    if (logLevel > ERROR)
    {
        return;
    }

    string logEntry = formatLogEntry(ERROR, time(0), message);
    if (shouldLogOnFile)
    {
        appendLineFileToFile(logFile.c_str(), logEntry);
    }
    else
    {
        cout << logEntry << endl;
    }
    // cout << "[ERROR] "  << time(0) << " " << message << endl;
}

void warn(string message)
{
    if (logLevel > WARN)
    {
        return;
    }

    string logEntry = formatLogEntry(WARN, time(0), message);
    if (shouldLogOnFile)
    {
        appendLineFileToFile(logFile.c_str(), logEntry);
    }
    else
    {
        cout << logEntry << endl;
    }
    // cout << "[WARN] "  << time(0) << " " << message << endl;
}

void info(string message)
{
    if (logLevel > INFO)
    {
        return;
    }

    string logEntry = formatLogEntry(INFO, time(0), message);
    if (shouldLogOnFile)
    {
        appendLineFileToFile(logFile.c_str(), logEntry);
    }
    else
    {
        cout << logEntry << endl;
    }
    // cout << "[INFO] "  << time(0) << " " << message << endl;
}

void debug(string message)
{
    if (logLevel > DEBUG)
    {
        return;
    }

    string logEntry = formatLogEntry(DEBUG, time(0), message);
    if (shouldLogOnFile)
    {
        appendLineFileToFile(logFile.c_str(), logEntry);
    }
    else
    {
        cout << logEntry << endl;
    }
    // cout << "[DEBUG] "  << time(0) << " " << message << endl;
}

void trace(string message)
{
    if (logLevel > TRACE)
    {
        return;
    }

    string logEntry = formatLogEntry(TRACE, time(0), message);
    if (shouldLogOnFile)
    {
        appendLineFileToFile(logFile.c_str(), logEntry);
    }
    else
    {
        cout << logEntry << endl;
    }
    // cout << "[TRACE] "  << time(0) << " " << message << endl;
}
