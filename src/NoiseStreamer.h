#ifndef NoiseStreamer_h__
#define NoiseStreamer_h__

#include <string>

using namespace std;

class NoiseStreamer
{
private:
    static const char* USER_AGENT;

public:
    NoiseStreamer();
    virtual ~NoiseStreamer();
};

#endif // NoiseStreamer_h__
