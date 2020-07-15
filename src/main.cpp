#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <fstream>
#include <sstream>

#include "settings.hpp"
#include "midi.hpp"
#include "output.hpp"
#include "wave.hpp"
#include "gui.hpp"


wave::Source *sourceBuilder() {
    std::ifstream infile("instrument.txt");
    std::string line;
    float args[5];
    wave::Source *s = nullptr;

    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string name;
        if (!(iss >> name)) break;
        if (name == "square") {
            iss >> args[0];
            iss >> args[1];
            iss >> args[2];
            iss >> args[3];
            iss >> args[4];
            s = new wave::SquareInstrument(args[0], args[1], args[2], args[3], args[4]);
        } else if (name == "saw") {
            iss >> args[0];
            iss >> args[1];
            iss >> args[2];
            iss >> args[3];
            iss >> args[4];
            s = new wave::SawInstrument(args[0], args[1], args[2], args[3], args[4]);
        } else if (name == "noise") {
            iss >> args[0];
            iss >> args[1];
            iss >> args[2];
            iss >> args[3];
            iss >> args[4];
            s = new wave::NoiseInstrument(args[0], args[1], args[2], args[3], args[4]);
        } else if (name == "sin") {
            iss >> args[0];
            iss >> args[1];
            iss >> args[2];
            iss >> args[3];
            iss >> args[4];
            s = new wave::SinInstrument(args[0], args[1], args[2], args[3], args[4]);
        } else if (name == "tremolo") {
            iss >> args[0];
            iss >> args[1];
            s = new wave::TremoloModifier(s, args[0], args[1]);
        } else if (name == "phase") {
            iss >> args[0];
            iss >> args[1];
            s = new wave::PhaseShiftModifier(s, args[0], args[1]);
        }  else if (name == "delay") {
            iss >> args[0];
            iss >> args[1];
            s = new wave::DelayModifier(s, args[0], args[1]);
        } else if (name == "gain") {
            iss >> args[0];
            s = new wave::GainModifier(s, args[0]);
        } else if (name == "pitch") {
            iss >> args[0];
            s = new wave::PitchModifier(s, args[0]);
        }
    }

    return s;
}

void printCode(unsigned char status) {
    for (int i = 0; i < 8; ++i) {
        std::string val = status & 0x80 ? "1" : "0";
        std::cout << val;
        status <<= 1;
    }
    std::cout << std::endl << std::flush;
}

int main() {
    try {
        wave::setSource(
            sourceBuilder()
        );

        gui::init();
        int MIDIChoice = gui::choose("Choose MIDI device:", midi::getMIDIOptions());
        midi::Sequencer *sequencer = midi::getSequencer(MIDIChoice);
        output::init();

        midi::pkt_t pkt;
        while(1) {
            pkt = sequencer->readPacket();
            if (pkt.status == midi::NOTE) wave::getSource()->addNote(pkt);
            else if (pkt.status == midi::NOTEOFF) wave::getSource()->removeNote(pkt);
            else if (pkt.status != midi::TIMING && pkt.status != 254) printCode(pkt.status);
        }
    } catch (char const* e) {
        gui::showError(e);
        gui::clear();
        exit(EXIT_FAILURE);
    }

    gui::clear();
    exit(EXIT_SUCCESS);
}