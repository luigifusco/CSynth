#pragma once

#include "midi.hpp"

#include <SDL2/SDL.h>
#include <set>
#include <mutex>


namespace wave {

class Note {
    public:
    float freq;
    int t;
    int vel;
    bool decaying;
    Note(float freq, int t, int vel, bool decaying);
    bool operator<(const Note& n) const;
    bool operator==(const Note& n) const;
};

class Source {
    public:
    virtual float getSample(int t) = 0;
    virtual void addNote(midi::pkt_t pkt) = 0;
    virtual void removeNote(midi::pkt_t pkt) = 0;
    virtual void clearNotes() = 0;
};

class Instrument: public Source {
    protected:
    float attack, decay, sustain, release;
    float getVolume(int dt, int velocity, bool decaying);
    public:
    Instrument(float a, float d, float s, float r);
    std::set<Note> notes;
    void addNote(midi::pkt_t pkt);
    void removeNote(midi::pkt_t pkt);
    void clearNotes();
};

class Modifier: public Source {
    protected:
    Source *src;
    public:
    Modifier(Source *s);
    void addNote(midi::pkt_t pkt);
    void removeNote(midi::pkt_t pkt);
    void clearNotes();
};

class TremoloModifier: public Modifier {
    private:
    float tremoloFrequency;
    float tremoloIntensity;
    public:
    TremoloModifier(Source *s, float tremolo, float intensity);
    float getSample(int t);
};

class PhaseShiftModifier: public Modifier {
    private:
    float shiftFrequency;
    float shiftIntensity;
    public:
    PhaseShiftModifier(Source *s, float shift, float intensity);
    float getSample(int t);
};

class DelayModifier: public Modifier {
    private:
    float delayIntensity;
    int len;
    float *delayBuffer;
    public:
    DelayModifier(Source *s, float length, float intensity);
    float getSample(int t);
};

class VoidInstrument: public Instrument {
    public:
    using Instrument::Instrument;
    float getSample(int t);
};

class SquareInstrument: public Instrument {
    public:
    using Instrument::Instrument;
    float getSample(int t);
};
class SinInstrument: public Instrument {
    public:
    using Instrument::Instrument;
    float getSample(int t);
};
class NoiseInstrument: public Instrument {
    public:
    using Instrument::Instrument;
    float getSample(int t);
};
class SawInstrument: public Instrument {
    public:
    using Instrument::Instrument;
    float getSample(int t);
};

Source *getSource();
void setSource(Source *inst);
void callback(void *user_data, Uint8 *raw_buffer, int bytes);

}
