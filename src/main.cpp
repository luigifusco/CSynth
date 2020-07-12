#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

#include "settings.hpp"
#include "midi.hpp"
#include "output.hpp"
#include "wave.hpp"


int main() {
    wave::mtx.lock();
    wave::instrument = new wave::SquareInstrument();
    wave::mtx.unlock();

    if (output::open_device() < 0) {
        std::cerr << "Error opening output" << std::endl;
        exit(EXIT_FAILURE);
    }    

    midi::MIDIDevice sequencer(MIDI_DEVICE);

    output::start();

    midi::midipkt_t pkt;
    while(1) {
        pkt = sequencer.readPacket();
        if (pkt.status == midi::NOTE || pkt.status == midi::NOTEOFF) {
            if (pkt.status == midi::NOTE) {
                if (pkt.data[1] > 0) {
                    std::cout <<
                        "PRESSED " <<
                        midi::getNoteId(pkt) <<
                        " (" << midi::getNoteFrequency(pkt) << ") " <<
                        (int)pkt.data[1] << std::endl << std::flush;
                    wave::instrument->addFrequency(midi::getNoteFrequency(pkt));
                } else {
                    std::cout <<
                        "RELEASED " <<
                        midi::getNoteId(pkt) <<
                        " (" << midi::getNoteFrequency(pkt) << ") " <<
                        (int)pkt.data[1] << std::endl << std::flush;
                    wave::instrument->removeFrequency(midi::getNoteFrequency(pkt));
                }
            } else if (pkt.status == midi::NOTEOFF) {
                std::cout << "RELEASED (ALT) " <<
                midi::getNoteId(pkt) <<
                        " (" << midi::getNoteFrequency(pkt) << ") " <<
                        (int)pkt.data[1] << std::endl << std::flush;
                wave::instrument->removeFrequency(midi::getNoteFrequency(pkt));
            }
        } else if (pkt.status == midi::STOP) {
            wave::instrument->clearFrequencies();
        } else if (pkt.status != midi::TIMING && pkt.status != 254) {
            for (int i = 0; i < 8; ++i) {
                std::string val = pkt.status & 0x80 ? "1" : "0";
                std::cout << val;
                pkt.status <<= 1;
            }
            std::cout << std::endl << std::flush;
        }
    }

    return 0;
}