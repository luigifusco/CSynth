#pragma once

#include <soundio/soundio.h>

int output_open_default_device(struct SoundIo *soundio, struct SoundIoOutStream *out);

void output_generator_noise(struct SoundIoOutStream *out, int frame_min, int frame_max);