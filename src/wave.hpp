#pragma once

#include <SDL2/SDL.h>
#include <set>
#include <mutex>


namespace wave {

class Source {
    public:
    virtual float getSample(int t) = 0;
    virtual void addFrequency(float f) = 0;
    virtual void removeFrequency(float f) = 0;
    virtual void clearFrequencies() = 0;
};

class Instrument: public Source {
    public:
    std::set<float> freqs;
    void addFrequency(float f);
    void removeFrequency(float f);
    void clearFrequencies();
};

class Modifier: public Source {
    protected:
    Source *src;
    public:
    Modifier(Source *s);
    void addFrequency(float f);
    void removeFrequency(float f);
    void clearFrequencies();
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
