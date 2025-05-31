#ifndef TrackMetadata_h__
#define TrackMetadata_h__

#include <string>

using namespace std;

struct TrackMetadata
{
    string filename;

    string title;
    string artist;
    string album;
    string comment;
    string genre;
    string year;

    int duration;
    int bitrate;
    int samplerate;
    int channels;
};

#endif // TrackMetadata_h__
