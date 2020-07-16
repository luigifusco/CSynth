#include "output.hpp"
#include "settings.hpp"
#include "wave.hpp"

#include <sys/soundcard.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>
#include <filesystem>


namespace output {

int sample_nr = 0;

void init() {
    if (SDL_Init(SDL_INIT_AUDIO)) throw "Error initializing SDL!";

    SDL_AudioSpec want = {
        .freq = SAMPLE_RATE,
        .format = AUDIO_F32SYS,
        .channels = 1,
        .samples = N_SAMPLES,
        .callback = wave::callback,
        .userdata = &sample_nr
    };

    SDL_AudioSpec have;
    
    if (SDL_OpenAudio(&want, &have) < 0) throw "Error opening output!";

    start();
}

void start() {
    SDL_PauseAudio(0);
}

void stop() {
    SDL_PauseAudio(1);
}

std::vector<std::string> getSourceOptions() {
    std::vector<std::string> options;
    for (const auto& entry : std::filesystem::directory_iterator("instruments"))
        options.push_back(entry.path());
    
    return options;
}

wave::Source *sourceBuilder(std::string filename) {
    std::ifstream infile(filename);
    std::string line;
    float args[5];
    wave::Source *s = nullptr;

    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string name;
        if (!(iss >> name)) break;
        if (name == "square") {
            iss >> args[0];
            iss >> args[1];
            iss >> args[2];
            iss >> args[3];
            iss >> args[4];
            s = new wave::SquareInstrument(args[0], args[1], args[2], args[3], args[4]);
        } else if (name == "saw") {
            iss >> args[0];
            iss >> args[1];
            iss >> args[2];
            iss >> args[3];
            iss >> args[4];
            s = new wave::SawInstrument(args[0], args[1], args[2], args[3], args[4]);
        } else if (name == "noise") {
            iss >> args[0];
            iss >> args[1];
            iss >> args[2];
            iss >> args[3];
            iss >> args[4];
            s = new wave::NoiseInstrument(args[0], args[1], args[2], args[3], args[4]);
        } else if (name == "sin") {
            iss >> args[0];
            iss >> args[1];
            iss >> args[2];
            iss >> args[3];
            iss >> args[4];
            s = new wave::SinInstrument(args[0], args[1], args[2], args[3], args[4]);
        } else if (name == "tremolo") {
            iss >> args[0];
            iss >> args[1];
            s = new wave::TremoloModifier(s, args[0], args[1]);
        } else if (name == "phase") {
            iss >> args[0];
            iss >> args[1];
            s = new wave::PhaseShiftModifier(s, args[0], args[1]);
        }  else if (name == "delay") {
            iss >> args[0];
            iss >> args[1];
            s = new wave::DelayModifier(s, args[0], args[1]);
        } else if (name == "gain") {
            iss >> args[0];
            s = new wave::GainModifier(s, args[0]);
        } else if (name == "pitch") {
            iss >> args[0];
            s = new wave::PitchModifier(s, args[0]);
        }
    }

    return s;
}


}
