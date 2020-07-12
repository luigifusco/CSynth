#pragma once

#include <string>

namespace midi {

const unsigned char NOTEOFF = 0x80;
const unsigned char NOTE = 0x90;
const unsigned char TIMING = 0xF0;
const unsigned char STOP = 0x50;

typedef struct {
    unsigned char status;
    unsigned char data[3];
} midipkt_t;

class MIDIDevice {
    private:
    int fd;
    public:
    MIDIDevice(std::string path);
    midipkt_t readPacket();
    int close_device();
};

std::string getNoteId(midipkt_t &pkt);
int getNoteTone(midipkt_t &pkt);
float getNoteFrequency(midipkt_t &pkt);

}