#include "wave.hpp"

#include "settings.hpp"
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <string.h>

namespace wave {

Instrument *instrument = new Instrument();
std::mutex mtx;

float Instrument::getSample(float t) {
    return 0;
}

float SquareInstrument::getSample(float t) {
    float val = 0.0f;
    for (float const& f : freqs) {
        float period = ((float) SAMPLE_RATE / f);
        val += ((float) (((int)t%(int)period > ((int)period/2)))) / 10.0f;
    }

    return val;
}

float SinInstrument::getSample(float t) {
    float val = 0.0f;
    for (float const& f : freqs) {
        float period = ((float) SAMPLE_RATE / f);
        val += (sinf(t * 2 * M_PI / period)) / 10.0f;
    }

    return val;
}

float NoiseInstrument::getSample(float t) {
    return (float) rand() / (float) RAND_MAX;
}

void callback(void *user_data, Uint8 *raw_buffer, int bytes) {
    float *buffer = (float*) raw_buffer;
    int len = bytes / 4;
    int sample_nr = *((int*)user_data);

    sample_nr = *((int*)user_data);

    mtx.lock();
    for (int i = 0; i < len; ++i, sample_nr++) {
        buffer[i] = instrument->getSample((float)sample_nr);
    }
    mtx.unlock();

    *((int*)user_data) = sample_nr;
}

void Instrument::addFrequency(float f) {
    mtx.lock();
    freqs.insert(f);
    mtx.unlock();
}

void Instrument::removeFrequency(float f) {
    mtx.lock();
    freqs.erase(f);
    mtx.unlock();
}

void Instrument::clearFrequencies() {
    mtx.lock();
    freqs.clear();
    mtx.unlock();
}

}
