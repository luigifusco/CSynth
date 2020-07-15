#include "wave.hpp"

#include "settings.hpp"
#include "output.hpp"
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <string.h>

namespace wave {

Source *globalSource = new VoidInstrument(0.0, 0.0, 0.0, 0.0);
std::mutex mtx;

inline float getPeriod(float f) {
    return ((float) SAMPLE_RATE / f);
}

void setSource(Source *inst) {
    mtx.lock();
    globalSource = inst;
    mtx.unlock();
}

Source *getSource() {
    return globalSource;
}


Note::Note(float freq, int t, int vel, bool decaying) {
    this->freq = freq; this->t = t; this->vel = vel; this->decaying = decaying;
}

bool Note::operator<(const Note& n) const {
    return freq < n.freq;
}

bool Note::operator==(const Note& n) const {
    return freq == n.freq;
}

Instrument::Instrument(float a, float d, float s, float r) {
    attack=a; decay=d; sustain=s; release=r;
}

float Instrument::getVolume(int dt, int velocity, bool decaying) {
    float time = (float) dt / (float) SAMPLE_RATE;
    float vel_factor = (float) velocity / (float) midi::VEL_MAX;
    if (decaying) {
        if (time < release) return vel_factor * sustain * (1 - (time / release));
        else return 0;
    }
    else if (time < attack) return vel_factor * time / attack;
    else if (time < attack + decay) return vel_factor * (1 - ((1 - sustain) * (time - attack) / decay)); 
    else return vel_factor * sustain;
}

float VoidInstrument::getSample(int t) {
    return 0;
}

float SquareInstrument::getSample(int t) {
    float val = 0.0f;
    for (Note const& n : notes) {
        float period = getPeriod(n.freq);
        val += getVolume(t - n.t, n.vel, n.decaying) * (((float) ((t%(int)period > ((int)period/2)))) / 10.0f);
    }

    return val;
}

float SawInstrument::getSample(int t) {
    float val = 0.0f;
    for (Note const& n : notes) {
        float period = getPeriod(n.freq);
        if (t - n.t < SAMPLE_RATE)
            val += ((float) n.vel / (float) midi::VEL_MAX) * (1.0f - ((float) (t - n.t) / (float) SAMPLE_RATE)) * ((float) ((int)t%(int)period * 2 / period) - 1) / 10.0f;
    }

    return val;
}

float SinInstrument::getSample(int t) {
    float val = 0.0f;
    for (Note const& n : notes) {
        float period = getPeriod(n.freq);
        if (t - n.t < SAMPLE_RATE)
            val += ((float) n.vel / (float) midi::VEL_MAX) * (1.0f - ((float) (t - n.t) / (float) SAMPLE_RATE)) * (sinf(t * 2 * M_PI / period)) / 10.0f;
    }

    return val;
}

float NoiseInstrument::getSample(int t) {
    if (notes.size() > 0)
        return ((float) rand() / (float) RAND_MAX);
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

void Instrument::addNote(midi::pkt_t pkt) {
    mtx.lock();
    Note n(midi::getNoteFrequency(pkt), output::sample_nr, pkt.data[1], false);
    if (pkt.data[1] == 0 && notes.find(n) != notes.end()) {
        n.decaying = true;
        n.vel = notes.find(n)->vel;
    }
    notes.erase(n);
    notes.insert(n);
    mtx.unlock();
}

void Instrument::removeNote(midi::pkt_t pkt) {
    mtx.lock();
    Note n(midi::getNoteFrequency(pkt), output::sample_nr, pkt.data[1], false);
    notes.erase(n);
    mtx.unlock();
}

void Instrument::clearNotes() {
    mtx.lock();
    notes.clear();
    mtx.unlock();
}

void Modifier::addNote(midi::pkt_t pkt) {
    src->addNote(pkt);
}

void Modifier::removeNote(midi::pkt_t pkt) {
    src->removeNote(pkt);
}

void Modifier::clearNotes() {
    src->clearNotes();
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
    float period = getPeriod(tremoloFrequency);
    float modifier = (sinf(t * 2 * M_PI / period) * tremoloIntensity / 2) + 1 - (tremoloIntensity / 2);

    return samp * modifier;
}

PhaseShiftModifier::PhaseShiftModifier(Source *s, float shift, float intensity) : Modifier(s) {
    shiftFrequency = shift;
    shiftIntensity = intensity;
}

DelayModifier::DelayModifier(Source *s, float length, float intensity) : Modifier(s) {
    delayIntensity = intensity;
    len = length * SAMPLE_RATE;
    delayBuffer = new float[len];
    for (int i = 0; i < len; ++i) delayBuffer[i] = 0.0f;
}

float DelayModifier::getSample(int t) {
    float oldSample = delayBuffer[t%len];
    delayBuffer[t%len] = src->getSample(t);

    return delayBuffer[t%len] + oldSample * delayIntensity;
}

float PhaseShiftModifier::getSample(int t) {
    float period = getPeriod(shiftFrequency);
    int modifier = sinf(t * 2 * M_PI / period) * shiftIntensity;
    
    return src->getSample(t + modifier);
}

}
