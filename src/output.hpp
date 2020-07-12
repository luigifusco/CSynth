#pragma once

#include <SDL2/SDL.h>

namespace output {
    
int open_device();

void start();
void stop();

}