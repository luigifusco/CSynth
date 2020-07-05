#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

#include "settings.h"
#include "midi.h"
#include "output.h"
#include "wave.h"

int main() {
    if (output_open_default_device(wave_generator_square_callback) < 0) {
        fprintf(stderr, "Error opening output\n");
        exit(EXIT_FAILURE);
    }    

    int seqfd = midi_open_sequencer(MIDI_DEVICE);
    if (seqfd < 0) {
        fprintf(stderr, "Error opening sequencer\n");
        exit(EXIT_FAILURE);
    }

    output_start();

    midipkt_t pkt;
    while(1) {
        if (midi_read_packet(seqfd, &pkt) < 0)  {
            fprintf(stderr, "Error reading packet\n");
            exit(EXIT_FAILURE);
        }
        if (pkt.status == MIDI_NOTE) {
            if (pkt.data[1] > 0) {
                float freq = midi_get_frequency(&pkt);
                wave_set_frequency(freq);
                output_start();
            }
        }
    }

    return 0;
}