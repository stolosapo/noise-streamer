#ifndef NoiseStreamerHealthPolicy_h__
#define NoiseStreamerHealthPolicy_h__

class NoiseStreamerHealthPolicy
{
private:
    static const int ERROR_THRESHOLD;
    static const int SHOUT_QUEUE_LENGTH_THRESHOLD;

    int shoutQueueLength;
    int errorCounter;

public:
    NoiseStreamerHealthPolicy();
    virtual ~NoiseStreamerHealthPolicy();

    virtual bool isHealthy();

    void setShoutQueueLenth(int queueLength);
    int getShoutQueueLength();
    void incrementErrorCounter();
    void resetErrorCounter();

    void assertErrorCounterThresholdReached();
    void assertShoutQueueLengthThresholdReached();
};

#endif // NoiseStreamerHealthPolicy_h__