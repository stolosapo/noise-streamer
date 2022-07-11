#include "NoiseStreamerHealthPolicy.h"
#include "../exception/NoiseStreamerException.h"
#include "../utils/StringHelper.h"

const int NoiseStreamerHealthPolicy::ERROR_THRESHOLD = 10;
const int NoiseStreamerHealthPolicy::SHOUT_QUEUE_LENGTH_THRESHOLD = 6000000;

NoiseStreamerHealthPolicy::NoiseStreamerHealthPolicy()
{
    resetErrorCounter();
}

NoiseStreamerHealthPolicy::~NoiseStreamerHealthPolicy()
{

}

bool NoiseStreamerHealthPolicy::isHealthy()
{
    return true;
}

void NoiseStreamerHealthPolicy::setShoutQueueLenth(int queueLength)
{
    shoutQueueLength = queueLength;
}

int NoiseStreamerHealthPolicy::getShoutQueueLength()
{
    return shoutQueueLength;
}

void NoiseStreamerHealthPolicy::incrementErrorCounter()
{
    errorCounter++;
}

void NoiseStreamerHealthPolicy::resetErrorCounter()
{
    this->errorCounter = 0;
}

void NoiseStreamerHealthPolicy::assertErrorCounterThresholdReached()
{
    if (errorCounter >= ERROR_THRESHOLD)
    {
        throw DomainException(NSS0021);
    }
}

void NoiseStreamerHealthPolicy::assertShoutQueueLengthThresholdReached()
{
    if (shoutQueueLength >= SHOUT_QUEUE_LENGTH_THRESHOLD)
    {
        throw DomainException(NSS0027, numberToString<int>(shoutQueueLength));
    }
}
