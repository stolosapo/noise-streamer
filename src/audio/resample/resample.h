#ifndef resample_h__
#define resample_h__

#include "../CircularBuffer.h"
#include "../CircularBuffer_v2.h"

void resample_pcm(CircularBuffer* input_pcm, CircularBuffer* output_pcm, int original_rate, int new_rate);
void resample_pcm(CircularBuffer_v2<short>* input_pcm, CircularBuffer_v2<short>* output_pcm, int original_rate, int new_rate);

#endif // resample_h__
