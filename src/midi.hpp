#pragma once

#include <string>
#include <vector>
#include <mutex>
#include <queue>
#include <condition_variable>

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

class Sequencer {
    public:
    virtual pkt_t readPacket() = 0;
    virtual int close_device() = 0;
};

class MIDISequencer : public Sequencer {
    private:
    int fd;
    public:
    MIDISequencer(std::string path);
    pkt_t readPacket();
    int close_device();
};

class KeyboardSequencer : public Sequencer {
    private:
    std::queue<unsigned char> q;
    std::condition_variable cv;
    std::mutex m;
    public:
    KeyboardSequencer();
    pkt_t readPacket();
    int close_device();
};

std::string getNoteId(pkt_t &pkt);
int getNoteTone(pkt_t &pkt);
float getNoteFrequency(pkt_t &pkt);
std::vector<std::string> getMIDIOptions();
Sequencer *getSequencer(int id);

}