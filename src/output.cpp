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
    int count = SDL_GetNumAudioDevices(0);
    std::cout << "Select audio device:" << std::endl;
    for (int i = 0; i < count; ++i) {
        std::cout << i << ") " << SDL_GetAudioDeviceName(i, 0) << std::endl;
    }
    int choice = -1;
    while (choice < 0 || choice >= count) {
        std::cout << "> ";
        std::cin >> choice;
    }

    return SDL_OpenAudioDevice(SDL_GetAudioDeviceName(choice, 0), 0, &want, &have, 0);
}

void start(int dev) {
    SDL_PauseAudioDevice(dev, 0);
}

void stop(int dev) {
    SDL_PauseAudioDevice(dev, 1);
}

}
