#pragma once

#include <SDL2/SDL.h>

namespace output {
    
void init();

void start();
void stop();

extern int sample_nr;

}