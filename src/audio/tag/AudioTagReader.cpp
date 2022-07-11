#include "AudioTagReader.h"

#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <string>

#include "../../utils/StringHelper.h"
#include "../../utils/FileHelper.h"

#ifdef HAVE_TAGLIB
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/tpropertymap.h>
#endif

using namespace std;

AudioTag* readAudioTag(const char* filepath)
{
    string title = "";
    string artist = "";
    string album = "";
    string comment = "";
    string genre = "";
    int year = 0;
    int track = 0;

    int duration = 0;
    string strDuration = "--:--";
    int bitrate = 0;
    int samplerate = 0;
    int channels = 0;

#ifdef HAVE_TAGLIB
    TagLib::FileRef f(filepath);

    if (!f.isNull() && f.tag())
    {
        TagLib::Tag *tag = f.tag();

        bool unicode = true;

        title = tag->title().toCString(unicode);
        artist = tag->artist().toCString(unicode);
        album = tag->album().toCString(unicode);
        comment = tag->comment().toCString(unicode);
        genre = tag->genre().toCString(unicode);
        year = tag->year();
        track = tag->track();

        // delete tag;
    }

    if (!f.isNull() && f.audioProperties())
    {
        TagLib::AudioProperties *properties = f.audioProperties();

        int sec = properties->lengthInSeconds();
        int hours = (sec / 60 / 60) % 24;
        int minutes = (sec / 60) % 60;
        int seconds = sec % 60;

        char s[25];
        sprintf(s, "%02d:%02d", minutes, seconds);

        strDuration = string(s);

        duration = properties->lengthInSeconds();
        bitrate = properties->bitrate();
        samplerate = properties->sampleRate();
        channels = properties->channels();

        // delete properties;
    }
#else
    title = filename(filepath);
	return AudioTag::emptyWithTitle(title);
#endif

    return new AudioTag(
        title,
        artist,
        album,
        comment,
        genre,
        year,
        track,
        duration,
        strDuration,
        bitrate,
        samplerate,
        channels
    );
}
