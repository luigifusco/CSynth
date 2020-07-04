#pragma once

#define MIDI_NOTEOFF 0x80
#define MIDI_NOTE 0x90

typedef struct {
    unsigned char status;
    unsigned char data[3];
} midipkt_t;


// opens a sequencer at specified path and returns its fileno, returns < 0 on error
int midi_open_sequencer(char *path);

// reads a packet from a sequencer, returns < 0 on failure
int midi_read_packet(int seqfd, midipkt_t *pkt);

// given a packet containing a note gets the note name and tone, returns < 0 on failure
int midi_get_note_id(midipkt_t *pkt, char const **note, int *tone);

int midi_close(int seqfd);