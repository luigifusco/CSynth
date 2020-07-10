#include "wave.hpp"

#include "settings.hpp"
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <set>
#include <iostream>
#include <string.h>

namespace wave {


std::set<float> freqs;


void noise_callback(void *user_data, Uint8 *raw_buffer, int bytes) {
    float *buffer = (float*) raw_buffer;
    int len = bytes / 4;
    int *sample_nr = (int*) user_data;

    for (int i = 0; i < len; ++i, (*sample_nr)++) {
        buffer[i] = (float) rand() / (float) RAND_MAX;
    }
}

void square_callback(void *user_data, Uint8 *raw_buffer, int bytes) {
    float *buffer = (float*) raw_buffer;
    int len = bytes / 4;
    int sample_nr = *((int*)user_data);

    memset(raw_buffer, 0, bytes);

    for (auto const& f : freqs) {
        sample_nr = *((int*)user_data);
        int period = (int) ((float) SAMPLE_RATE / f);

        for (int i = 0; i < len; ++i, sample_nr++) {
            buffer[i] += ((float) ((sample_nr%period > (period/2)))) / 10.0f;
        }
    }

    *((int*)user_data) = sample_nr;
}

void sin_callback(void *user_data, Uint8 *raw_buffer, int bytes) {
    float *buffer = (float*) raw_buffer;
    int len = bytes / 4;
    int sample_nr = *((int*)user_data);

    memset(raw_buffer, 0, bytes);

    for (auto const& f : freqs) {
        sample_nr = *((int*)user_data);
        float period = (float) SAMPLE_RATE / f;

        for (int i = 0; i < len; ++i, sample_nr++) {
            buffer[i] += (sinf((float)sample_nr * M_PI / period)) / 10.0f;
        }
    }

    *((int*)user_data) = sample_nr;
}

void set_frequency(float freq) {
    freqs.insert(freq);
}

void unset_frequency(float freq) {
    freqs.erase(freq);
}

}
