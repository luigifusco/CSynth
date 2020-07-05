#pragma once

#include <SDL2/SDL.h>

void wave_generator_square_callback(void *user_data, Uint8 *raw_buffer, int bytes);

void wave_set_frequency(float freq);
