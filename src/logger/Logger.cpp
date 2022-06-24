#include "Logger.h"
#include <iostream>

using namespace std;

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
    cout << "[ERROR] "  << time(0) << " " << message << endl;
}

void warn(string message)
{
    cout << "[WARN] "  << time(0) << " " << message << endl;
}

void info(string message)
{
    cout << "[INFO] "  << time(0) << " " << message << endl;
}

void debug(string message)
{
    cout << "[DEBUG] "  << time(0) << " " << message << endl;
}

void trace(string message)
{
    cout << "[TRACE] "  << time(0) << " " << message << endl;
}
