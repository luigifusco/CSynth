#pragma once

#include <SDL2/SDL.h>


int wave_init();

void wave_generator_square_callback(void *user_data, Uint8 *raw_buffer, int bytes);

void wave_add_note(float freq);

void wave_remove_note(float freq);
