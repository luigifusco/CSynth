#pragma once

#include <string>

namespace midi {

const int VEL_MAX = 127;

const unsigned char NOTEOFF = 0x80;
const unsigned char NOTE = 0x90;
const unsigned char TIMING = 0xF0;
const unsigned char STOP_A = 0x50;
const unsigned char STOP_B = 0x30;
const unsigned char STOP_C = 0x40;

typedef struct {
    unsigned char status;
    unsigned char data[3];
} pkt_t;

class MIDIDevice {
    private:
    int fd;
    public:
    MIDIDevice(std::string path);
    pkt_t readPacket();
    int close_device();
};

std::string getNoteId(pkt_t &pkt);
int getNoteTone(pkt_t &pkt);
float getNoteFrequency(pkt_t &pkt);

}