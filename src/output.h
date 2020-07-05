#pragma once

#include <SDL2/SDL.h>

int output_open_default_device(void (*callback)(void*, Uint8*, int));

void output_start();
void output_stop();