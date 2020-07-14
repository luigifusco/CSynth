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
    Note(float freq, int t, int vel) {
        this->freq = freq; this->t = t; this->vel = vel;
    }
    bool operator<(const Note& n) const {
        return freq < n.freq;
    }
    bool operator==(const Note& n) const {
        return freq == n.freq;
    }
};

class Source {
    public:
    virtual float getSample(int t) = 0;
    virtual void addNote(midi::pkt_t pkt) = 0;
    virtual void removeNote(midi::pkt_t pkt) = 0;
    virtual void clearNotes() = 0;
};

class Instrument: public Source {
    public:
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

class VoidInstrument: public Instrument {
    public:
    float getSample(int t);
};

class SquareInstrument: public Instrument {
    public:
    float getSample(int t);
};
class SinInstrument: public Instrument {
    public:
    float getSample(int t);
};
class NoiseInstrument: public Instrument {
    public:
    float getSample(int t);
};
class SawInstrument: public Instrument {
    public:
    float getSample(int t);
};

Source *getSource();
void setSource(Source *inst);
void callback(void *user_data, Uint8 *raw_buffer, int bytes);

}
