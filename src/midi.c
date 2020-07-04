#include "midi.h"

#include <sys/soundcard.h>
#include <fcntl.h>
#include <unistd.h>

int midi_open_sequencer(char *path) {
    return open(path, O_RDONLY);
}

int midi_read_packet(int seqfd, midipkt_t *pkt) {
    while(1) {
        int nbytes = read(seqfd, pkt, sizeof(midipkt_t));
        if (nbytes < 0) return nbytes;
        if (pkt->status == SEQ_MIDIPUTC) return 0;
    }
}

int midi_close(int seqfd) {
    return close(seqfd);
}
