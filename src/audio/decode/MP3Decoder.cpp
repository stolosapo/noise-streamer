#include "MP3Decoder.h"

#include <iostream>
#include <string>
#include <vector>
#include "../../exception/NoiseStreamerException.h"
#include "../../utils/StringHelper.h"

using namespace std;

MP3Decoder::MP3Decoder(SignalAdapter* sigSrv, CircularBuffer<short>* outputBuffer)
    : AudioDecoder(sigSrv, outputBuffer)
{
// #ifdef HAVE_MPG123
    mh = NULL;

    int err = mpg123_init();
    if (err != MPG123_OK)
    {
        throw DomainException(DEC0002, mpg123_plain_strerror(err)); 
    }
// #endif
}

MP3Decoder::~MP3Decoder()
{
    close();
    mpg123_exit();
}

bool MP3Decoder::open(const char* filename)
{
    int err;
    mh = mpg123_new(NULL, &err);
    if (!mh)
    {
        throw DomainException(DEC0003, mpg123_plain_strerror(err)); 
    }

    // Enable ID3v1 and ID3v2 tag reading
    mpg123_param(mh, MPG123_ADD_FLAGS, MPG123_IGNORE_INFOFRAME, 0.);
    mpg123_param(mh, MPG123_ADD_FLAGS, MPG123_AUTO_RESAMPLE, 0.);
    

    if (mpg123_open(mh, filename) != MPG123_OK) 
    {
        throw DomainException(DEC0004, mpg123_strerror(mh)); 
    }

    return true;
}

void MP3Decoder::close()
{
    if (mh) 
    {
        mpg123_close(mh);
        mpg123_delete(mh);
        mh = NULL;
    }
}

bool MP3Decoder::decode()
{
    int err = MPG123_OK;
    long rate = getSampleRate();
    int channels = getChannels();

    // Ensure output format is signed 16-bit integers
    mpg123_format_none(mh);
    if (mpg123_format(mh, rate, channels, MPG123_ENC_SIGNED_16) != MPG123_OK)
    {
        throw DomainException(DEC0005, mpg123_strerror(mh)); 
    }

    // Prepare buffer and read loop
    size_t buffer_size = mpg123_outblock(mh);
    std::vector<unsigned char> read_buffer(buffer_size);
    size_t done = 0;

    // Read and decode the entire file
    do
    {
        err = mpg123_read(mh, &read_buffer[0], read_buffer.size(), &done);
        
        if (done > 0)
        {
            // Convert to short* (16-bit samples) and write to circular buffer
            size_t sample_count = done / sizeof(short);
            if (!outputBuffer->write(reinterpret_cast<short*>(&read_buffer[0]), sample_count))
            {
                // Buffer was closed
                break;
            }
        }
    } while (err == MPG123_OK && done > 0);

    if (err != MPG123_DONE && err != MPG123_OK)
    {
        throw DomainException(DEC0006, mpg123_strerror(mh)); 
    }

    return true;
}

int MP3Decoder::getSampleRate() const
{
    long rate;
    if (mpg123_getformat(mh, &rate, NULL, NULL) != MPG123_OK)
    {
        throw DomainException(DEC0005, mpg123_strerror(mh)); 
    }
    return static_cast<int>(rate);
}

int MP3Decoder::getChannels() const
{
    int channels;
    if (mpg123_getformat(mh, NULL, &channels, NULL) != MPG123_OK)
    {
        throw DomainException(DEC0005, mpg123_strerror(mh)); 
    }
    return channels;
}

int64_t MP3Decoder::getTotalSamples() const
{
    return mpg123_length(mh);
}

bool MP3Decoder::getMetadata(TrackMetadata &metadata) const
{
    mpg123_id3v1 *v1;
    mpg123_id3v2 *v2;
    
    if(mpg123_scan(mh) != MPG123_OK) 
    {
        throw DomainException(DEC0007, mpg123_strerror(mh));
    }

    if(mpg123_id3(mh, &v1, &v2) != MPG123_OK) 
    {
        throw DomainException(DEC0008);
    }

    int64_t frames = getTotalSamples();
    int rate = getSampleRate();
    int duration = (frames > 0) ? static_cast<int>(frames) / rate : -1;

    mpg123_frameinfo frameinfo;
    if(mpg123_info(mh, &frameinfo) != MPG123_OK) {
        throw DomainException(DEC0009);
    }

    if (v2->title != NULL)
    {
        metadata.title = string(v2->title->p);
    }

    if (v2->album != NULL)
    {
        metadata.album = string(v2->album->p);
    }

    if (v2->artist != NULL)
    {
        metadata.artist = string(v2->artist->p);
    }

    if (v2->comment != NULL)
    {
        metadata.comment = string(v2->comment->p);
    }

    if (v2->genre != NULL)
    {
        metadata.genre = string(v2->genre->p);
    }

    if (v2->year != NULL)
    {
        metadata.year = string(v2->year->p);
    }

    metadata.duration = duration;
    metadata.bitrate = frameinfo.bitrate;
    metadata.samplerate = rate;
    metadata.channels = getChannels();

    return true;
}