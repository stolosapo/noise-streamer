#include "resample.h"
#include <samplerate.h>
#include "../CircularBuffer.h"
#include "../BufferSizes.h"

void resample_pcm(CircularBuffer* input_pcm, CircularBuffer* output_pcm, int original_rate, int new_rate)
{  
    const size_t CHUNK = RESAMPLE_CHUNK;
    
    size_t input_size = CHUNK * 2; // Stereo input
    short input[input_size];
    
    size_t output_size = (input_size * new_rate) / original_rate; // Extra space for upsampling
    float output[output_size]; 

    input_pcm->read(input, input_size);

    const float* inputFloat = reinterpret_cast<const float*>(input);

    SRC_DATA src_data;
    src_data.data_in = inputFloat;
    src_data.input_frames = input_size / 2; // Assuming stereo
    src_data.data_out = output;
    src_data.output_frames = output_size / 2;
    src_data.src_ratio = (double) new_rate / original_rate;

    src_simple(&src_data, SRC_SINC_BEST_QUALITY, 2);

    output_pcm->write(reinterpret_cast<const short*>(output), src_data.output_frames_gen * 2);
    
    // cout << "(resample_pcm) Read: " << input << " Write: " << output << endl;
}

void resample_pcm(CircularBuffer_v2<short>* input_pcm, CircularBuffer_v2<short>* output_pcm, int original_rate, int new_rate)
{
    const size_t CHUNK = RESAMPLE_CHUNK;
    
    size_t input_size = CHUNK * 2; // Stereo input
    short input[input_size];
    
    size_t output_size = (input_size * new_rate) / original_rate; // Extra space for upsampling
    float output[output_size]; 

    size_t read = input_pcm->read(input, input_size);

    const float* inputFloat = reinterpret_cast<const float*>(input);

    SRC_DATA src_data;
    src_data.data_in = inputFloat;
    src_data.input_frames = input_size / 2; // Assuming stereo
    src_data.data_out = output;
    src_data.output_frames = output_size / 2;
    src_data.src_ratio = (double) new_rate / original_rate;

    src_simple(&src_data, SRC_SINC_BEST_QUALITY, 2);

    output_pcm->write(reinterpret_cast<const short*>(output), src_data.output_frames_gen * 2);
    
    // cout << "(resample_pcm) Read: " << input << " Write: " << output << endl;
}