#include "NoiseStreamerArgument.h"

const string NoiseStreamerArgument::HOSTNAME = "hostname";
const string NoiseStreamerArgument::PORT = "port";
const string NoiseStreamerArgument::USERNAME = "username";
const string NoiseStreamerArgument::PASSWORD = "password";
const string NoiseStreamerArgument::MOUNTPOINT = "mountpoint";
const string NoiseStreamerArgument::NAME = "name";
const string NoiseStreamerArgument::GENRE = "genre";
const string NoiseStreamerArgument::DESCRIPTION = "description";
const string NoiseStreamerArgument::URL = "url";
const string NoiseStreamerArgument::ISPUBLIC = "public";
const string NoiseStreamerArgument::BITRATE = "bitrate";
const string NoiseStreamerArgument::SAMPLERATE = "samplerate";
const string NoiseStreamerArgument::CHANNELS = "channels";

NoiseStreamerArgument::NoiseStreamerArgument(
    ArgumentProvider* argProvider): ArgumentAdapter(argProvider)
{

}

NoiseStreamerArgument::~NoiseStreamerArgument()
{

}

string NoiseStreamerArgument::title()
{
    return "noisestreamer help";
}

void NoiseStreamerArgument::registerArguments()
{
    registerArg(HOSTNAME, "The Icecast hostname");
    registerArg(PORT, "The Icecast port");
    registerArg(USERNAME, "The Mountpoint username");
    registerArg(PASSWORD, "The Mountpoint password");
    registerArg(MOUNTPOINT, "The Mountpoint itself");
    registerArg(NAME, "The Mountpoint Name");
    registerArg(GENRE, "The Mountpoint Genre");
    registerArg(DESCRIPTION, "The Mountpoint Description");
    registerArg(URL, "A Url for the radio station");
    registerArg(ISPUBLIC, "If Mountpoint is Public, true - false");
    registerArg(BITRATE, "The Mountpoint Bitrate");
    registerArg(SAMPLERATE, "The Mountpoint Samplerate");
    registerArg(CHANNELS, "The Mountpoint Number of channels");
}

string NoiseStreamerArgument::getHostname()
{
    return getStringValue(HOSTNAME);
}

string NoiseStreamerArgument::getPort()
{
    return getStringValue(PORT);
}

string NoiseStreamerArgument::getUsername()
{
    return getStringValue(USERNAME);
}

string NoiseStreamerArgument::getPassword()
{
    return getStringValue(PASSWORD);
}

string NoiseStreamerArgument::getMountpoint()
{
    return getStringValue(MOUNTPOINT);
}

string NoiseStreamerArgument::getName()
{
    return getStringValue(NAME);
}

string NoiseStreamerArgument::getGenre()
{
    return getStringValue(GENRE);
}

string NoiseStreamerArgument::getDescription()
{
    return getStringValue(DESCRIPTION);
}

string NoiseStreamerArgument::getUrl()
{
    return getStringValue(URL);
}

bool NoiseStreamerArgument::getIsPublic()
{
    return getBoolValue(ISPUBLIC);
}

int NoiseStreamerArgument::getBitrate()
{
    return getIntValue(BITRATE);
}

string NoiseStreamerArgument::getSamplerate()
{
    return getStringValue(SAMPLERATE);
}

string NoiseStreamerArgument::getChannels()
{
    return getStringValue(CHANNELS);
}
