#ifndef RunMode_h__
#define RunMode_h__

#include <string>

using namespace std;

enum RunMode
{
    STANDALONE = 0,

    INTERACTIVE = 1,

    AGENT = 2,

    CLIENT = 3
};

RunMode runModeFromString(string runMode);
string runModeToString(RunMode runMode);

#endif // RunMode_h__
