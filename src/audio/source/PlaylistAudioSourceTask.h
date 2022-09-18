#ifndef PlaylistAudioSourceTask_h__
#define PlaylistAudioSourceTask_h__

#include <noisekernel/Thread.h>
#include "../playlist/PlaylistItem.h"

using namespace NoiseKernel;

TaskRunner* createPlaylistAudioSourceTaskRunner();
string createPlaylistAudioSourceTaskRunnerHelp();

string playlistaudiosource_format_playlistitem(PlaylistItem* track);

void* playlistaudiosource_audio_status(void* task);
void* playlistaudiosource_now_playing(void* task);
void* playlistaudiosource_preview_next(void* task);
void* playlistaudiosource_preview_track(void* task);
void* playlistaudiosource_history(void* task);
void* playlistaudiosource_next_track(void* task);
void* playlistaudiosource_request_track(void* task);

#endif // PlaylistAudioSourceTask_h__
