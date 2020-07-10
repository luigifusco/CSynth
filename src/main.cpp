#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

#include "settings.h"
#include "midi.h"
#include "output.h"
#include "wave.h"

int main() {
    int output_device = output::open_device(wave_generator_square_callback;
    if (!output_device) {
        std::cerr << "Error opening output" << std::endl;
        exit(EXIT_FAILURE);
    }    

    int seqfd = midi_open_sequencer(MIDI_DEVICE);
    if (seqfd < 0) {
        std::cerr << "Error opening sequencer" << std::endl;
        exit(EXIT_FAILURE);
    }

    output::start(output_device);

    midipkt_t pkt;
    while(1) {
        if (midi_read_packet(seqfd, &pkt) < 0)  {
            std::cerr << "Error reading packet" << std::endl;
            exit(EXIT_FAILURE);
        }
        if (pkt.status == MIDI_NOTE) {
            if (pkt.data[1] > 0) {
                char const *id;
                midi_get_note_id(&pkt, &id, NULL);
                float freq = midi_get_frequency(&pkt);
                std::cout << "PRESSED " << id << " (" << freq << ")" << std::endl << std::flush;
                wave_set_frequency(freq);
            }
        }
    }

    return 0;
}