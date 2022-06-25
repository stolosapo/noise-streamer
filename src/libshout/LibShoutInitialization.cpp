#include "LibShout.h"

void LibShout::shoutInit()
{
	shout_init();
}

void LibShout::shoutShutdown()
{
	shout_shutdown();
}

string LibShout::shoutVersion(int *major, int *minor, int *patch)
{
	const char* version = shout_version(major, minor, patch);
	return string(version);
}

string LibShout::shoutFullVersion()
{
	int major;
	int minor;
	int patch;
	string version = shoutVersion(&major, &minor, &patch);
	return "libshout/" + version;
}

void LibShout::shoutNew()
{
	if (!(shout = shout_new()))
	{
		// throw DomainException(NoiseStreamerDomainErrorCode::NSS0002);
	}
}

void LibShout::shoutFree()
{
	shout_free(shout);
}
