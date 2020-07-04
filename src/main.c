#include <stdio.h>
#include <unistd.h>

#include "settings.h"
#include "midi.h"

int main() {
    int seqfd;
    midipkt_t pkt;

    seqfd = midi_open_sequencer(MIDI_DEVICE);
    if (seqfd < 0) exit(1);
    while(1) {
        if (midi_read_packet(seqfd, &pkt) < 0) exit(1);
        printf("received MIDI byte: %d\n", pkt.data1);
    }

    return 0;
}