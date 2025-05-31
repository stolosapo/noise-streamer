#include "AudioDecoderFactory.h"

#include <string>
#include <algorithm>
#include <cctype>
#include "../../utils/FileHelper.h"
#include "MP3Decoder.h"
#include "../../exception/NoiseStreamerException.h"

using namespace std;

AudioDecoder* createDecoder(const char* filename, SignalAdapter* sigSrv, CircularBuffer<short>* outputBuffer)
{
    string ext = extension(filename);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    if (ext == "mp3")
    {
        return new MP3Decoder(sigSrv, outputBuffer);
    }

    throw DomainException(DEC0001, filename);
}
