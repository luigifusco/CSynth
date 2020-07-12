#pragma once

#include <SDL2/SDL.h>
#include <set>
#include <mutex>


namespace wave {

class Instrument {
    public:
    std::set<float> freqs;
    virtual float getSample(float t);
    void addFrequency(float f);
    void removeFrequency(float f);
    void clearFrequencies();
};

class SquareInstrument: public Instrument {
    public:
    float getSample(float t);
};
class SinInstrument: public Instrument {
    public:
    float getSample(float t);
};
class NoiseInstrument: public Instrument {
    public:
    float getSample(float t);
};

void callback(void *user_data, Uint8 *raw_buffer, int bytes);

extern Instrument *instrument;
extern std::mutex mtx;

}
