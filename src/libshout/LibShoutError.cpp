#include "LibShout.h"

const char* LibShout::getError()
{
	return shout_get_error(shout);
}

int LibShout::getErrno()
{
	return shout_get_errno(shout);
}
