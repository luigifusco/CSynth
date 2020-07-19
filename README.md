# CSynth

CSynth is a modular sound synthesizer written in C++ supporting MIDI and Keyboard input.
The instrument can be customly defined in the `instrument.txt` file, specifying the
single components and their arguments.

### Dependencies
The projects depends on `SDL2` for audio output and `ncurses` for input.
Dependencies can be installed in a Debian environment with:
```bash
sudo apt install libsdl2-dev libncurses-dev libsdl2-ttf-dev
```

### Compilation
The project can be compiled with the following command:
```bash
g++ src/*.cpp -lSDL2 -lSDL2_ttf -lm -lncurses -lpthread -o csynth
```