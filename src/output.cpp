#include "output.hpp"
#include "settings.hpp"
#include "wave.hpp"

#include <sys/soundcard.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <iostream>


namespace output {

int sample_nr = 0;

int open_device() {
    if (SDL_Init(SDL_INIT_AUDIO)) return -1;

    SDL_AudioSpec want = {
        .freq = SAMPLE_RATE,
        .format = AUDIO_F32SYS,
        .channels = 1,
        .samples = N_SAMPLES,
        .callback = wave::callback,
        .userdata = &sample_nr
    };

    SDL_AudioSpec have;
    
    if (SDL_OpenAudio(&want, &have) < 0) return -1;

    return 0;
}

void start() {
    SDL_PauseAudio(0);
}

void stop() {
    SDL_PauseAudio(1);
}

}
