#include <iostream>
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


wave::Source *sourceBuilder() {
    std::ifstream infile("settings.txt");
    std::string line;
    float args[2];
    wave::Source *s = nullptr;

    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string name;
        if (!(iss >> name)) break;
        if (name == "square") {
            s = new wave::SquareInstrument();
        } else if (name == "saw") {
            s = new wave::SawInstrument();
        } else if (name == "noise") {
            s = new wave::NoiseInstrument();
        } else if (name == "sin") {
            s = new wave::SinInstrument();
        } else if (name == "tremolo") {
            iss >> args[0];
            iss >> args[1];
            s = new wave::TremoloModifier(s, args[0], args[1]);
        } else if (name == "phase") {
            iss >> args[0];
            iss >> args[1];
            s = new wave::PhaseShiftModifier(s, args[0], args[1]);
        }
    }

    return s;
}

int main() {
    try {
        wave::setSource(
            sourceBuilder()
        );

        output::open_device();

        midi::MIDIDevice sequencer(MIDI_DEVICE);

        output::start();

        midi::pkt_t pkt;
        while(1) {
            pkt = sequencer.readPacket();
            if (pkt.status == midi::NOTE) {
                wave::getSource()->addNote(pkt);
            } else if (pkt.status == midi::NOTEOFF) {
                wave::getSource()->removeNote(pkt);
            } else if (pkt.status != midi::TIMING && pkt.status != 254) {
                for (int i = 0; i < 8; ++i) {
                    std::string val = pkt.status & 0x80 ? "1" : "0";
                    std::cout << val;
                    pkt.status <<= 1;
                }
                std::cout << std::endl << std::flush;
            }
        }
    } catch (char const* e) {
        std::cerr << e << std::endl;
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}