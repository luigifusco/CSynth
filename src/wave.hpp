#pragma once

#include <SDL2/SDL.h>

namespace wave {

void noise_callback(void *user_data, Uint8 *raw_buffer, int bytes);
void square_callback(void *user_data, Uint8 *raw_buffer, int bytes);
void sin_callback(void *user_data, Uint8 *raw_buffer, int bytes);

void set_frequency(float freq);
void unset_frequency(float freq);

}
