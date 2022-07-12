#include "NoiseStreamerArgument.h"
#include "../exception/NoiseStreamerException.h"

const string NoiseStreamerArgument::LOGLEVEL = "loglevel";
const string NoiseStreamerArgument::BACKGROUND = "background";
const string NoiseStreamerArgument::PIDFILE = "pidfile";
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
    registerArg(LOGLEVEL, "The LogLevel: TRACE, DEBUG, INFO, WARN, ERROR, FATAL. Default: INFO");
    registerArg(BACKGROUND, "If want to run application in the background");
    registerArg(PIDFILE, "The PID file that contains the background process id");
    registerArg(HOSTNAME, "The Icecast hostname. Default: '127.0.0.1'");
    registerArg(PORT, "The Icecast port. Default: '8000'");
    registerArg(USERNAME, "The Mountpoint username. Default: 'source'");
    registerArg(PASSWORD, "The Mountpoint password");
    registerArg(MOUNTPOINT, "The Mountpoint itself");
    registerArg(NAME, "The Mountpoint Name");
    registerArg(GENRE, "The Mountpoint Genre");
    registerArg(DESCRIPTION, "The Mountpoint Description");
    registerArg(URL, "A Url for the radio station");
    registerArg(ISPUBLIC, "If Mountpoint is Public. Default: false");
    registerArg(BITRATE, "The Mountpoint Bitrate. Default: '128'");
    registerArg(SAMPLERATE, "The Mountpoint Samplerate. Default: '44100'");
    registerArg(CHANNELS, "The Mountpoint Number of channels. Default: '2'");
}

bool NoiseStreamerArgument::noArgs()
{
    return !(hasArg(LOGLEVEL) ||
        hasArg(BACKGROUND) ||
        hasArg(PIDFILE) ||
        hasArg(HOSTNAME) ||
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

LogLevel NoiseStreamerArgument::getLogLevel()
{
    string s = getStringValue(LOGLEVEL);
    return convertLogLevelFromString(s);
}

bool NoiseStreamerArgument::runOnBackground()
{
    bool b = hasArg(BACKGROUND);
    if (b && !hasArg(PIDFILE))
    {
        throw DomainException(ARG0001, PIDFILE);
    }
    return b;
}

string NoiseStreamerArgument::pidFile()
{
    string s = getStringValue(PIDFILE);
    if (runOnBackground() && s == "")
    {
        throw DomainException(ARG0001, PIDFILE);
    }
    return s;
}

string NoiseStreamerArgument::getHostname()
{
    if (!hasArg(HOSTNAME))
    {
        return "127.0.0.1";
    }

    string s = getStringValue(HOSTNAME);
    if (s == "")
    {
        throw DomainException(ARG0001, HOSTNAME);
    }
    return s;
}

string NoiseStreamerArgument::getPort()
{
    if (!hasArg(PORT))
    {
        return "8000";
    }

    string s = getStringValue(PORT);
    if (s == "")
    {
        throw DomainException(ARG0001, PORT);
    }
    return s;
}

string NoiseStreamerArgument::getUsername()
{
    if (!hasArg(USERNAME))
    {
        return "source";
    }

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
    if (!hasArg(ISPUBLIC))
    {
        return false;
    }

    return getBoolValue(ISPUBLIC);
}

int NoiseStreamerArgument::getBitrate()
{
    if (!hasArg(BITRATE))
    {
        return 128;
    }

    int s = getIntValue(BITRATE);
    if (s == 0)
    {
        throw DomainException(ARG0001, BITRATE);
    }
    return s;
}

string NoiseStreamerArgument::getSamplerate()
{
    if (!hasArg(SAMPLERATE))
    {
        return "44100";
    }

    string s = getStringValue(SAMPLERATE);
    if (s == "")
    {
        throw DomainException(ARG0001, SAMPLERATE);
    }
    return s;
}

string NoiseStreamerArgument::getChannels()
{
    if (!hasArg(CHANNELS))
    {
        return "2";
    }

    string s = getStringValue(CHANNELS);
    if (s == "")
    {
        throw DomainException(ARG0001, CHANNELS);
    }
    return s;
}
