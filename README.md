# CSynth

CSynth is a modular sound synthesizer written in C++ supporting MIDI and Keyboard input.
Instruments can be customly defined in the `instruments` folder, specifying the
single components and their arguments. Some instruments are already defined as examples.

### Dependencies
The projects depends on `SDL2` for everything IO related.
Dependencies can be installed in a Debian environment with:
```bash
sudo apt install libsdl2-dev libsdl2-ttf-dev
```

### Compilation
The project can be compiled with the following command:
```bash
g++ src/*.cpp -lSDL2 -lSDL2_ttf -lm -lpthread -o csynth
```