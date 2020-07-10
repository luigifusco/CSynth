#pragma once

#include <SDL2/SDL.h>

namespace output {
    
int open_device(void (*callback)(void*, Uint8*, int));

void start();
void stop();

}