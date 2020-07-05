#include "output.h"
#include "settings.h"

#include <sys/soundcard.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <stdio.h>

int sample_nr = 0;

int output_open_default_device(void (*callback)(void*, Uint8*, int)) {
    if (SDL_Init(SDL_INIT_AUDIO)) return -1;

    SDL_AudioSpec want = {
        .freq = SAMPLE_RATE,
        .format = AUDIO_F32SYS,
        .channels = 1,
        .samples = N_SAMPLES,
        .callback = callback,
        .userdata = &sample_nr
    };

    SDL_AudioSpec have;
    if (SDL_OpenAudio(&want, &have)) return -1;
    if (want.format != have.format) return -1;

    return 0;
}

void output_start() {
    SDL_PauseAudio(0);
}

void output_stop() {
    SDL_PauseAudio(1);
}
