#pragma once

#include <SDL2/SDL.h>
#include "wave.hpp"

namespace output {
    
void init();

void start();
void stop();

std::vector<std::string> getSourceOptions();
wave::Source *sourceBuilder(std::string filename);

extern int sample_nr;

}