#include "PlaylistAudioSourceTask.h"
#include <time.h>
#include "PlaylistAudioSource.h"
#include "../../exception/NoiseStreamerException.h"
#include "../../utils/StringHelper.h"
#include "../../utils/TaskHelper.h"

TaskRunner* createPlaylistAudioSourceTaskRunner()
{
    TaskRunner* runner = new TaskRunner;
    runner->registerTask("playlist::audio-status", &playlistaudiosource_audio_status);
    runner->registerTask("playlist::now", &playlistaudiosource_now_playing);
    runner->registerTask("playlist::preview", &playlistaudiosource_preview_next);
    runner->registerTask("playlist::preview-track", &playlistaudiosource_preview_track);
    runner->registerTask("playlist::history", &playlistaudiosource_history);
    runner->registerTask("playlist::next", &playlistaudiosource_next_track);
    runner->registerTask("playlist::request", &playlistaudiosource_request_track);
    return runner;
}

string createPlaylistAudioSourceTaskRunnerHelp()
{
    return "\n"
	"Playlist :: Tasks\n"
	"=================\n"
	"\n"
    "playlist::audio-status             See the status of the Audio Source\n"
    "playlist::now                      See the current playing track\n"
    "playlist::preview                  Preview next track\n"
    "playlist::preview-track            Preview track by index\n"
    "playlist::history?[length=5]       Return the history of given length. Default is the last 5 tracks\n"
    "playlist::next                     Navigate to the next track\n"
    "playlist::request?[track_index]    Create a track request based on track index\n"
    "\n";
}

string playlistaudiosource_format_playlistitem(PlaylistItem* track, int trackStartTime)
{
    AudioTag* tag = track->getMetadata();
    int index = track->getTrackIndex();

    int progress = difftime(time(0), trackStartTime);
    int duration = tag->getDuration();
    int remaining = duration - progress;
    int minutes = (remaining / 60) % 60;
    int seconds = remaining % 60;

    char s[25];
    sprintf(s, "%02d:%02d", minutes, seconds);
    string strRemaining(s);

    string value = "\n";

    value += "Index: " + numberToString<int>(index) + "\n";
    value += "Track: " + track->getTrack() + "\n";
    value += "Title: " + tag->getTitle() + "\n";
    value += "Artist: " + tag->getArtist() + "\n";
    value += "Album: " + tag->getAlbum() + "\n";
    value += "Genre: " + tag->getGenre() + "\n";
    value += "Duration: " + tag->getStrDuration() + "\n";
    value += "Remaining: " + strRemaining + "\n";
    value += "Bitrate: " + numberToString<int>(tag->getBitrate()) + "\n";
    value += "Samplerate: " + numberToString<int>(tag->getSamplerate()) + "\n";
    value += "Channels: " + numberToString<int>(tag->getChannels()) + "\n";

    return value;
}

void* playlistaudiosource_audio_status(void* task)
{
    PlaylistAudioSource* source = unwrap_task<PlaylistAudioSource>(task);

    int numOfTracks = source->numberOfPlayedTracks;
    int historySize = source->playlistHandler->historySize();
    int playlistSize = source->playlistHandler->playlistSize();
    int queueSize = source->mainQueue.size();
    int requestQueueSize = source->requestedTrackIndex.size();

    string value = "\n";

    value += "Playlist Size: " + numberToString<int>(playlistSize) + "\n";
    value += "History Size: " + numberToString<int>(historySize) + "\n";
    value += "Number of played tracks: " + numberToString<int>(numOfTracks) + "\n";
    value += "Playlist Queue Size: " + numberToString<int>(queueSize) + "\n";
    value += "Request Queue Size: " + numberToString<int>(requestQueueSize) + "\n";

    return static_cast<void*>(new string(value));
}

void* playlistaudiosource_now_playing(void* task)
{
    PlaylistAudioSource* source = unwrap_task<PlaylistAudioSource>(task);
    PlaylistItem track = source->currentTrack;
    string value = playlistaudiosource_format_playlistitem(&track, source->currentTrackStartTime);
    return static_cast<void*>(new string(value));
}

void* playlistaudiosource_preview_next(void* task)
{
    PlaylistAudioSource* source = unwrap_task<PlaylistAudioSource>(task);
    PlaylistItem track = source->mainQueue.front()->getTrack();
    string value = playlistaudiosource_format_playlistitem(&track, -1);
    return static_cast<void*>(new string(value));
}

void* playlistaudiosource_preview_track(void* task)
{
    Task* t = (Task*) task;
    PlaylistAudioSource* source = (PlaylistAudioSource*) t->getData();
    string indexParam = t->getParam(0);
    if (indexParam == "")
    {
        return NULL;
    }

    int trackIndex = stringToNumber<int>(indexParam);
    PlaylistItem track = source->playlistHandler->getTrack(trackIndex);
    string value = playlistaudiosource_format_playlistitem(&track, -1);
    return static_cast<void*>(new string(value));
}

void* playlistaudiosource_history(void* task)
{
    Task* t = (Task*) task;
    PlaylistAudioSource* source = (PlaylistAudioSource*) t->getData();
    string lengthParam = t->getParam(0);
    int historySize = source->playlistHandler->historySize();
    if (historySize == 0)
    {
        return static_cast<void*>(new string(""));
    }

    int historyLength = 5;
    if (lengthParam != "")
    {
        historyLength = stringToNumber<int>(lengthParam);
    }

    if (historyLength >= historySize)
    {
        historyLength = historySize;
    }

    string value = "\n";

    for (int i = 0; i < historyLength; i++)
    {
        int inverseIndex = (historySize - 1) - i;

        string track =  source->playlistHandler->getHistoryTrack(inverseIndex);
        int plsIndex = source->playlistHandler->getTrackIndex(track);

        value += numberToString<int>(plsIndex) + ": " + track; + "\n";
    }

    return static_cast<void*>(new string(value));
}

void* playlistaudiosource_next_track(void* task)
{
    PlaylistAudioSource* playlistSource = unwrap_task<PlaylistAudioSource>(task);
    playlistSource->next();
    return NULL;
}

void* playlistaudiosource_request_track(void* task)
{
    Task* t = (Task*) task;
    PlaylistAudioSource* source = (PlaylistAudioSource*) t->getData();
    string indexParam = t->getParam(0);
    if (indexParam == "")
    {
        return NULL;
    }

    int index = stringToNumber<int>(indexParam);
    if (index >= source->playlistHandler->playlistSize() || index < 0)
    {
        throw DomainException(NSS0025);
    }

    source->requestedTrackIndex.putBack(index);

    return NULL;
}
