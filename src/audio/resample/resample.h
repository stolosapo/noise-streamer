#ifndef resample_h__
#define resample_h__

#include "../CircularBuffer.h"

void resample_pcm(CircularBuffer* input_pcm, CircularBuffer* output_pcm, int original_rate, int new_rate);

#endif // resample_h__
