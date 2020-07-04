#include "midi.h"

#include <sys/soundcard.h>
#include <fcntl.h>
#include <unistd.h>

#define MIDI_BASE 21

int midi_open_sequencer(char *path) {
    return open(path, O_RDONLY);
}

int midi_read_packet(int seqfd, midipkt_t *pkt) {
    int err = read(seqfd, pkt, sizeof(midipkt_t));

    return err;
}

const char const *note_codes[] = {
    "A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#"
};

int midi_get_note_id(midipkt_t *pkt, char const **note, int *tone) {
    if (pkt->data[0] < MIDI_BASE) return -1;
    *note = note_codes[(pkt->data[0] - MIDI_BASE)%12];
    *tone = pkt->data[0]/12;

    return 0;
}

int midi_close(int seqfd) {
    return close(seqfd);
}
