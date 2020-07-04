#pragma once

typedef struct {
    unsigned char   status,
                    data1,
                    data2,
                    data3
} midipkt_t;


// opens a sequencer at specified path and returns its fileno, returns < 0 on error
int midi_open_sequencer(char *path);

// reads a packet from a sequencer, returns < 0 on failure
int midi_read_packet(int seqfd, midipkt_t *pkt);

int midi_close(int seqfd);