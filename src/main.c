#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "settings.h"
#include "midi.h"
#include "output.h"

int main() {
    struct SoundIo *soundio;
    struct SoundIoOutStream *out;
    if (output_open_default_device(soundio, out) < 0) {
        fprintf(stderr, "Error opening soundio\n");
        exit(EXIT_FAILURE);
    }

    out->write_callback = output_generator_noise;

    if (out->layout_error) exit(EXIT_FAILURE);
    if (soundio_outstream_start(out)) exit(EXIT_FAILURE);

    while(1) {
        soundio_wait_events(soundio);
    }


    int seqfd = midi_open_sequencer(MIDI_DEVICE);
    if (seqfd < 0) {
        fprintf(stderr, "Error opening sequencer\n");
        exit(EXIT_FAILURE);
    }

    midipkt_t pkt;
    while(1) {
        if (midi_read_packet(seqfd, &pkt) < 0)  {
            fprintf(stderr, "Error reading packet\n");
            exit(EXIT_FAILURE);
        }
        if (pkt.status == MIDI_NOTE) {
            char const *note;
            int tone;
            midi_get_note_id(&pkt, &note, &tone);
        }
    }

    return 0;
}