#include "LibShout.h"

int LibShout::shoutSend(const unsigned char *data, size_t len)
{
    if (!successLastAction())
    {
        return 0;
    }

    int res = shout_send(shout, data, len);

    return res;
}

int LibShout::shoutQueuelen()
{
    if (!successLastAction())
    {
        return 0;
    }

    int res = (int) shout_queuelen(shout);

    return res;
}

void LibShout::shoutSync()
{
    if (!successLastAction())
    {
        return;
    }

    shout_sync(shout);
}

int LibShout::shoutDelay()
{
    if (!successLastAction())
    {
        return 0;
    }

    int res = shout_delay(shout);

    return res;
}
