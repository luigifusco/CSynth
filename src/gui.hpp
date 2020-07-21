#pragma once

#include <string>
#include <vector>
#include <functional>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace gui {

typedef SDL_Event Event;

void showMessage(std::string m);
void addCallback(Uint32 key, std::function<void(Event &e)> val);
void init();
void quit();
void showError(std::string err);
int choose(std::string prompt, std::vector<std::string> choices);

}