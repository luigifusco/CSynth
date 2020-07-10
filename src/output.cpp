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
    if (SDL_OpenAudioDevice(SDL_GetAudioDeviceName(choice, 0), 0, &want, &have, 0)) return -1;

    return 0;
}

void start() {
    SDL_PauseAudio(0);
    SDL_Delay(1000);
}

void stop() {
    SDL_PauseAudio(1);
}

}
