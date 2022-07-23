#include "LibLame.h"
#include <iostream>
#include <string>
#include "../../../exception/NoiseStreamerException.h"
#include "../../../logger/Logger.h"

using namespace std;

LibLame::LibLame()
{
#ifdef HAVE_LAME
    lame = NULL;
#endif
}

LibLame::~LibLame()
{
#ifdef HAVE_LAME
    if (lame != NULL)
    {
        // delete lame;
        // lame = NULL;
    }
#endif
}

const char* LibLame::version()
{
#ifdef HAVE_LAME
    return get_lame_version();
#endif
    return "";
}

void LibLame::init()
{
#ifdef HAVE_LAME
    lame = lame_init();
#endif
}

int LibLame::initParams()
{
#ifdef HAVE_LAME
    int r = lame_init_params(lame);
    if (r == -1)
    {
        throw DomainException(ENC0001);
    }
    return r;
#endif
    return -1;
}

void LibLame::close()
{
#ifdef HAVE_LAME
    lame_close(lame);
#endif
}

void LibLame::id3tagInit()
{
#ifdef HAVE_LAME
    id3tag_init(lame);
#endif
}

void LibLame::id3tagV2Only()
{
#ifdef HAVE_LAME
    id3tag_v2_only(lame);
#endif
}

void LibLame::id3tagSetYear(string year)
{
#ifdef HAVE_LAME
    id3tag_set_year(lame, year.c_str());
#endif
}

void LibLame::id3tagSetGenre(string genre)
{
#ifdef HAVE_LAME
    id3tag_set_genre(lame, genre.c_str());
#endif
}

void LibLame::id3tagSetArtist(string artist)
{
#ifdef HAVE_LAME
    id3tag_set_artist(lame, artist.c_str());
#endif
}

void LibLame::id3tagSetAlbum(string album)
{
#ifdef HAVE_LAME
    id3tag_set_album(lame, album.c_str());
#endif
}

void LibLame::id3tagSetTitle(string title)
{
#ifdef HAVE_LAME
    id3tag_set_title(lame, title.c_str());
#endif
}

void LibLame::id3tagSetTrack(string track)
{
#ifdef HAVE_LAME
    id3tag_set_track(lame, track.c_str());
#endif
}

void LibLame::id3tagSetComment(string comment)
{
#ifdef HAVE_LAME
    id3tag_set_comment(lame, comment.c_str());
#endif
}

void LibLame::id3tagSetAlbumArt(string albumArtPath)
{
#ifdef HAVE_LAME
    // set_id3_albumart(lame, albumArtPath.c_str());
#endif
}

int LibLame::getId3v1Tag(unsigned char* buffer, int size)
{
#ifdef HAVE_LAME
    return lame_get_id3v1_tag(lame, buffer, size);
#endif
    return -1;
}

int LibLame::getId3v2Tag(unsigned char* buffer, int size)
{
#ifdef HAVE_LAME
    return lame_get_id3v2_tag(lame, buffer, size);
#endif
    return -1;
}

int LibLame::getLameTagFrame(unsigned char* buffer, int size)
{
#ifdef HAVE_LAME
    return lame_get_lametag_frame(lame, buffer, size);
#endif
    return -1;
}

void LibLame::setDecodeOnly(bool enabled)
{
#ifdef HAVE_LAME
    lame_set_decode_only(lame, (int) enabled);
#endif
}

void LibLame::setInSamplerate(int samplerate)
{
#ifdef HAVE_LAME
    lame_set_in_samplerate(lame, samplerate);
#endif
}

void LibLame::setModeMono()
{
#ifdef HAVE_LAME
    lame_set_mode(lame, MONO);
#endif
}

void LibLame::setModeJointStereo()
{
#ifdef HAVE_LAME
    lame_set_mode(lame, JOINT_STEREO);
#endif
}

void LibLame::setNumChannels(int channels)
{
#ifdef HAVE_LAME
    lame_set_num_channels(lame, channels);
#endif
}

void LibLame::setBWriteVbrTag(bool enabled)
{
#ifdef HAVE_LAME
    lame_set_bWriteVbrTag(lame, enabled);
#endif
}

void LibLame::setVBR_ABR()
{
#ifdef HAVE_LAME
    lame_set_VBR(lame, vbr_abr);
#endif
}

void LibLame::setVBR_OFF()
{
#ifdef HAVE_LAME
    lame_set_VBR(lame, vbr_off);
#endif
}

void LibLame::setVBR_MTRH()
{
#ifdef HAVE_LAME
    lame_set_VBR(lame, vbr_mtrh);
#endif
}

void LibLame::setVBRq(int quality)
{
#ifdef HAVE_LAME
    lame_set_VBR_q(lame, quality);
#endif
}

void LibLame::setVBRMeanBitrateKbps(int abr_bitrate)
{
#ifdef HAVE_LAME
    lame_set_VBR_mean_bitrate_kbps(lame, abr_bitrate);
#endif
}

void LibLame::setQuality(int quality)
{
#ifdef HAVE_LAME
    lame_set_quality(lame, quality);
#endif
}

void LibLame::setBrate(int cbr_bitrate)
{
#ifdef HAVE_LAME
    lame_set_brate(lame, cbr_bitrate);
#endif
}

void LibLame::setOutSamplerate(int rample_frequency)
{
#ifdef HAVE_LAME
    lame_set_out_samplerate(lame, rample_frequency);
#endif
}

void LibLame::setFindReplayGain(bool enabled)
{
#ifdef HAVE_LAME
    lame_set_findReplayGain(lame, (int) enabled);
#endif
}

void LibLame::setWriteId3tagAutomatic(bool enabled)
{
#ifdef HAVE_LAME
    lame_set_write_id3tag_automatic(lame, (int) enabled);
#endif
}

#ifdef HAVE_LAME
hip_t LibLame::hipDecodeInit()
{
    hip_t hip = hip_decode_init();

    hip_set_errorf(hip, &decodeReportError);
    hip_set_debugf(hip, &decodeReportDebug);
    hip_set_msgf(hip, &decodeReportMessage);

    return hip;
}

void LibLame::hipDecodeExit(hip_t hip)
{
    hip_decode_exit(hip);
}

int LibLame::hipDecode(hip_t hip, unsigned char*  mp3buf, size_t len, short pcm_l[], short pcm_r[])
{
    /*
    -1: decoding error
     0: need more data before we can complete the decode
    >0: returned 'nout' samples worth of data in pcm_l,pcm_r
    */
    return hip_decode(hip, mp3buf, len, pcm_l, pcm_r);
}

int LibLame::hipDecodeHeaders(hip_t hip, unsigned char*  mp3buf, size_t len, short pcm_l[], short pcm_r[], mp3data_struct* mp3data)
{
    /*
    -1: decoding error
     0: need more data before we can complete the decode
    >0: returned 'nout' samples worth of data in pcm_l,pcm_r
    */
    return hip_decode_headers(hip, mp3buf, len, pcm_l, pcm_r, mp3data);
}

int LibLame::hipDecode1Headers(hip_t hip, unsigned char* mp3_buffer, size_t mp3_len, short pcm_l[], short pcm_r[], mp3data_struct* mp3data)
{
    /*
    -1: decoding error
     0: need more data before we can complete the decode
    >0: returned 'nout' samples worth of data in pcm_l,pcm_r
    */
    return hip_decode1_headers(hip, mp3_buffer, mp3_len, pcm_l, pcm_r, mp3data);
}

void LibLame::decodeReportError(const char *format, va_list ap)
{
    rootLogService.error("LibShout Decode: " + string(format));
}

void LibLame::decodeReportDebug(const char *format, va_list ap)
{
    rootLogService.debug("LibShout Decode: " + string(format));
}

void LibLame::decodeReportMessage(const char *format, va_list ap)
{
    rootLogService.info("LibShout Decode: " + string(format));
}
#endif


int LibLame::encodeFlush(unsigned char* mp3_buffer, int size)
{
#ifdef HAVE_LAME
    return lame_encode_flush(lame, mp3_buffer, size);
#endif
    return -1;
}

int LibLame::encodeBuffer(short int* pcm_buffer_l, short int* pcm_buffer_r, int num_samples, unsigned char* mp3_buffer, int mp3_buffer_size)
{
#ifdef HAVE_LAME
    /*
    * return code number of bytes output in mp3buf. Can be 0
    *             -1:  mp3buf was too small
    *             -2:  malloc() problem
    *             -3:  lame_init_params() not called
    *             -4:  psycho acoustic problems
    */
    return lame_encode_buffer(lame, pcm_buffer_l, pcm_buffer_r, num_samples, mp3_buffer, mp3_buffer_size);
#endif
    return -1;
}

int LibLame::encodeBufferInterleaved(short int* pcm_buffer, int num_samples, unsigned char* mp3_buffer, int mp3_buffer_size)
{
#ifdef HAVE_LAME
    return lame_encode_buffer_interleaved(lame, pcm_buffer, num_samples, mp3_buffer, mp3_buffer_size);
#endif
    return -1;
}
