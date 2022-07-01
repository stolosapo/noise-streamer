#ifndef PlaylistAudioSourceNavigator_h__
#define PlaylistAudioSourceNavigator_h__

#include <signal.h>

class PlaylistAudioSourceNavigator
{
private:
    volatile sig_atomic_t _stop;
    volatile sig_atomic_t _next;

public:
    PlaylistAudioSourceNavigator();
    virtual ~PlaylistAudioSourceNavigator();

    void stop();
    void next();
    void resetNext();

    bool isStoped();
    bool isGoToNext();
};

#endif // PlaylistAudioSourceNavigator_h__
