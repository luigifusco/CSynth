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
        gui::init();

        auto sourceOptions = output::getSourceOptions();
        int sourceChoice = gui::choose("Choose instrument:", sourceOptions);
        wave::setSource(output::sourceBuilder(sourceOptions[sourceChoice]));

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
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}