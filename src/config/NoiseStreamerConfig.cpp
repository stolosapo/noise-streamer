#include "NoiseStreamerConfig.h"
#include "../audio/AudioBitrate.h"

NoiseStreamerConfig buildNoiseStreamerConfig(NoiseStreamerArgument* args)
{
    NoiseStreamerConfig conf;

    conf.hostname = args->getHostname();
    conf.port = args->getPort();
    conf.username = args->getUsername();
    conf.password = args->getPassword();
    conf.protocol = "";
    conf.mountpoint = args->getMountpoint();
    conf.name = args->getName();
    conf.genre = args->getGenre();
    conf.description = args->getDescription();
    conf.url = args ->getUrl();

    if (args->getIsPublic())
    {
        conf.isPublic = "1";
    } else {
        conf.isPublic = "0";
    }

    conf.bitrate = convert(args->getBitrate());
    conf.samplerate = args->getSamplerate();
    conf.channels = args->getChannels();

    return conf;
}
