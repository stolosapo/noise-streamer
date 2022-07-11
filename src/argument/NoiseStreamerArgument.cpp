#include "NoiseStreamerArgument.h"
#include "../exception/NoiseStreamerException.h"

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
    return "NoiseStreamer help";
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

bool NoiseStreamerArgument::noArgs()
{
    return !(hasArg(HOSTNAME) ||
        hasArg(PORT) ||
        hasArg(USERNAME) ||
        hasArg(PASSWORD) ||
        hasArg(MOUNTPOINT) ||
        hasArg(NAME) ||
        hasArg(GENRE) ||
        hasArg(DESCRIPTION) ||
        hasArg(URL) ||
        hasArg(ISPUBLIC) ||
        hasArg(BITRATE) ||
        hasArg(SAMPLERATE) ||
        hasArg(CHANNELS));
}

string NoiseStreamerArgument::getHostname()
{
    string s = getStringValue(HOSTNAME);
    if (s == "")
    {
        throw DomainException(ARG0001, HOSTNAME);
    }
    return s;
}

string NoiseStreamerArgument::getPort()
{
    string s = getStringValue(PORT);
    if (s == "")
    {
        throw DomainException(ARG0001, PORT);
    }
    return s;
}

string NoiseStreamerArgument::getUsername()
{
    string s = getStringValue(USERNAME);
    if (s == "")
    {
        throw DomainException(ARG0001, USERNAME);
    }
    return s;
}

string NoiseStreamerArgument::getPassword()
{
    string s = getStringValue(PASSWORD);
    if (s == "")
    {
        throw DomainException(ARG0001, PASSWORD);
    }
    return s;
}

string NoiseStreamerArgument::getMountpoint()
{
    string s = getStringValue(MOUNTPOINT);
    if (s == "")
    {
        throw DomainException(ARG0001, MOUNTPOINT);
    }
    return s;
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
    int s = getIntValue(BITRATE);
    if (s == 0)
    {
        throw DomainException(ARG0001, BITRATE);
    }
    return s;
}

string NoiseStreamerArgument::getSamplerate()
{
    string s = getStringValue(SAMPLERATE);
    if (s == "")
    {
        throw DomainException(ARG0001, SAMPLERATE);
    }
    return s;
}

string NoiseStreamerArgument::getChannels()
{
    string s = getStringValue(CHANNELS);
    if (s == "")
    {
        throw DomainException(ARG0001, CHANNELS);
    }
    return s;
}
