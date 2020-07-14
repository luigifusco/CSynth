#include "wave.hpp"

#include "settings.hpp"
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <string.h>

namespace wave {

Source *globalSource = new VoidInstrument();
std::mutex mtx;

void setSource(Source *inst) {
    mtx.lock();
    globalSource = inst;
    mtx.unlock();
}

Source *getSource() {
    return globalSource;
}

float VoidInstrument::getSample(int t) {
    return 0;
}

float SquareInstrument::getSample(int t) {
    float val = 0.0f;
    for (float const& f : freqs) {
        float period = ((float) SAMPLE_RATE / f);
        val += ((float) ((t%(int)period > ((int)period/2)))) / 10.0f;
    }

    return val;
}

float SawInstrument::getSample(int t) {
    float val = 0.0f;
    for (float const& f : freqs) {
        float period = ((float) SAMPLE_RATE / f);
        val += ((float) ((int)t%(int)period * 2 / period) - 1) / 10.0f;
    }

    return val;
}

float SinInstrument::getSample(int t) {
    float val = 0.0f;
    for (float const& f : freqs) {
        float period = ((float) SAMPLE_RATE / f);
        val += (sinf(t * 2 * M_PI / period)) / 10.0f;
    }

    return val;
}

float NoiseInstrument::getSample(int t) {
    if (freqs.size() > 0)
        return (float) rand() / (float) RAND_MAX;
    return 0;
}

void callback(void *user_data, Uint8 *raw_buffer, int bytes) {
    float *buffer = (float*) raw_buffer;
    int len = bytes / 4;
    int sample_nr = *((int*)user_data);

    sample_nr = *((int*)user_data);

    mtx.lock();
    for (int i = 0; i < len; ++i, sample_nr++) {
        buffer[i] = globalSource->getSample(sample_nr);
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

void Modifier::addFrequency(float f) {
    src->addFrequency(f);
}

void Modifier::removeFrequency(float f) {
    src->removeFrequency(f);
}

void Modifier::clearFrequencies() {
    src->clearFrequencies();
}

Modifier::Modifier(Source *s) {
    src = s;
}

TremoloModifier::TremoloModifier(Source *s, float tremolo, float intensity) : Modifier(s) {
    tremoloFrequency = tremolo;
    tremoloIntensity = intensity;
}

float TremoloModifier::getSample(int t) {
    float samp = src->getSample(t);
    float period = ((float) SAMPLE_RATE / tremoloFrequency);
    float modifier = (sinf(t * 2 * M_PI / period) * tremoloIntensity / 2) + 1 - (tremoloIntensity / 2);

    return samp * modifier;
}

PhaseShiftModifier::PhaseShiftModifier(Source *s, float shift, float intensity) : Modifier(s) {
    shiftFrequency = shift;
    shiftIntensity = intensity;
}

float PhaseShiftModifier::getSample(int t) {
    float period = ((float) SAMPLE_RATE / shiftFrequency);
    int modifier = sinf(t * 2 * M_PI / period) * shiftIntensity;
    
    return src->getSample(t + modifier);
}

}
