#include "midi.hpp"

#include <sys/soundcard.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <string>

namespace midi {

static const int MIDI_BASE = 21;
static const int MIDI_A4 = 69;

static const float note_step = 1.059463094f;

MIDIDevice::MIDIDevice(std::string path) {
    fd = open(path.c_str(), O_RDONLY);
    if (fd < 0) throw "Invalid MIDI device!";
}

pkt_t MIDIDevice::readPacket() {
    pkt_t pkt;
    if (read(fd, &pkt, sizeof(pkt_t)) < 0)
        throw "Error reading MIDI packet!";
    pkt.status &= 0xF0;

    return pkt;
}

const std::string note_codes[] = {
    "A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#"
};

std::string getNoteId(pkt_t &pkt) {
    if (pkt.data[0] < MIDI_BASE) throw "Invalid note!";
    
    return note_codes[(pkt.data[0] - MIDI_BASE)%12];
}

int getNoteTone(pkt_t &pkt) {
    if (pkt.data[0] < MIDI_BASE) throw "Invalid note!";
    
    return pkt.data[0]/12;
}

float getNoteFrequency(pkt_t &pkt) {
    float diff = pkt.data[0] - MIDI_A4;

    return 440.0f * powf(note_step, diff);
}

int MIDIDevice::close_device() {
    return close(fd);
}

}
