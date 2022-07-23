#include "RunMode.h"

RunMode runModeFromString(string runMode)
{
    if (runMode == "STANDALONE")
    {
        return STANDALONE;
    }

    if (runMode == "INTERACTIVE")
    {
        return INTERACTIVE;
    }

    if (runMode == "AGENT")
    {
        return AGENT;
    }

    if (runMode == "CLIENT")
    {
        return CLIENT;
    }

    return STANDALONE;
}

string runModeToString(RunMode runMode)
{
    switch (runMode)
    {
    case STANDALONE:
        return "STANDALONE";
    case INTERACTIVE:
        return "INTERACTIVE";
    case AGENT:
        return "AGENT";
    case CLIENT:
        return "CLIENT";
    default:
        return "";
    }
}
