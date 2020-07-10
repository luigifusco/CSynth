#include "output.h"
#include "settings.h"

#include <sys/soundcard.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <iostream>


namespace output {

int sample_nr = 0;

int open_device(void (*callback)(void*, Uint8*, int)) {
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
    
    return SDL_OpenAudio(&want, &have);
}

void start() {
    SDL_PauseAudio(0);
}

void stop() {
    SDL_PauseAudio(1);
}

}
