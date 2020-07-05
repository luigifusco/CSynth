#include "wave.h"

#include "settings.h"
#include <math.h>

typedef struct {
    float freq;
} wave_options_t;

wave_options_t global_options = {
    .freq = 440.0f
};

void wave_set_frequency(float freq) {
    global_options.freq = freq;
}

void wave_generator_square_callback(void *user_data, Uint8 *raw_buffer, int bytes) {
    float *buffer = (float*) raw_buffer;
    int len = bytes / 4;
    int *sample_nr = (int*) user_data;

    int period = (int) ((float) SAMPLE_RATE / global_options.freq);

    for (int i = 0; i < len; ++i, (*sample_nr)++) {
        buffer[i] = (float) ((*sample_nr%period > (period/2)));
    }
}