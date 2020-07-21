#include "midi.hpp"

#include "gui.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <ncurses.h>

namespace midi {

static const int MIDI_BASE = 21;
static const int MIDI_A4 = 69;

static const float note_step = 1.059463094f;

MIDISequencer::MIDISequencer(std::string path) {
    fd = open(path.c_str(), O_RDONLY);
    if (fd < 0) throw "Invalid MIDI device!";
}

pkt_t MIDISequencer::readPacket() {
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

int MIDISequencer::close_device() {
    return close(fd);
}

unsigned char charToMIDI(char c) {
    switch (c) {
        case 'q': return MIDI_A4-21;
        case '2': return MIDI_A4-20;
        case 'w': return MIDI_A4-19;
        case '3': return MIDI_A4-18;
        case 'e': return MIDI_A4-17;
        case 'r': return MIDI_A4-16;
        case '5': return MIDI_A4-15;
        case 't': return MIDI_A4-14;
        case '6': return MIDI_A4-13;
        case 'y': return MIDI_A4-12;
        case '7': return MIDI_A4-11;
        case 'u': return MIDI_A4-10;
        case 'z': return MIDI_A4-9;
        case 's': return MIDI_A4-8;
        case 'x': return MIDI_A4-7;
        case 'd': return MIDI_A4-6;
        case 'c': return MIDI_A4-5;
        case 'v': return MIDI_A4-4;
        case 'g': return MIDI_A4-3;
        case 'b': return MIDI_A4-2;
        case 'h': return MIDI_A4-1;
        case 'n': return MIDI_A4;
        case 'j': return MIDI_A4+1;
        case 'm': return MIDI_A4+2;
        default: return 0;
   }
}

KeyboardSequencer::KeyboardSequencer() {
    gui::addCallback(SDL_KEYDOWN, [this](gui::Event& e){
        unsigned char midinote = charToMIDI(e.key.keysym.sym);
        if (midinote && !e.key.repeat) {
            std::unique_lock<std::mutex> lck(this->m);
            this->q.push(midinote);
            this->q.push(127);
            this->cv.notify_all();
        }
    });
    gui::addCallback(SDL_KEYUP, [this](gui::Event& e){
        unsigned char midinote = charToMIDI(e.key.keysym.sym);
        if (midinote) {
            std::unique_lock<std::mutex> lck(this->m);
            this->q.push(midinote);
            this->q.push(0);
            this->cv.notify_all();   
        }
    });
}

pkt_t KeyboardSequencer::readPacket() {
    pkt_t pkt;
    pkt.status = NOTE;
    std::unique_lock<std::mutex> lck(m);
    while (q.empty())
        cv.wait(lck);

    pkt.data[0] = q.front();
    q.pop();
    pkt.data[1] = q.front();
    q.pop();

    return pkt;
}

int KeyboardSequencer::close_device() { return 0; }

std::vector<std::string> getMIDIInternalDevices() {
    std::vector<std::string> options;
    for (const auto& entry : std::filesystem::directory_iterator("/dev/snd"))
        if (entry.path().string().find("midi") != std::string::npos)
            options.push_back(entry.path());
    
    return options;
}

std::vector<std::string> getMIDIOptions() {
    std::vector<std::string> options{"Keyboard"};
    std::vector<std::string> internal = getMIDIInternalDevices();
    options.insert(options.end(), internal.begin(), internal.end());

    return options;
}

Sequencer *getSequencer(int id) {
    if (id == 0) return new KeyboardSequencer();
    else
        return new MIDISequencer(getMIDIInternalDevices()[id-1]);
}

}
