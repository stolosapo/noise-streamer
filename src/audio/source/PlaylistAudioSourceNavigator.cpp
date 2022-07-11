#include "PlaylistAudioSourceNavigator.h"

#include <unistd.h>

PlaylistAudioSourceNavigator::PlaylistAudioSourceNavigator()
{
    _stop = 0;
    _next = 0;
}

PlaylistAudioSourceNavigator::~PlaylistAudioSourceNavigator()
{

}

void PlaylistAudioSourceNavigator::stop()
{
    _stop = 1;
}

void PlaylistAudioSourceNavigator::next()
{
    _next = 1;
}

void PlaylistAudioSourceNavigator::resetNext()
{
    _next = 0;
}

bool PlaylistAudioSourceNavigator::isStoped()
{
    return _stop == 1;
}

bool PlaylistAudioSourceNavigator::isGoToNext()
{
    return _next == 1;
}
