#include "gui.hpp"

#include "settings.hpp"

#include <iostream>
#include <string>
#include <mutex>
#include <thread>
#include <map>
#include <functional>

namespace gui {

TTF_Font *font = NULL;
SDL_Renderer *renderer = NULL;
SDL_Window *window = NULL;
std::mutex eventMTX;
std::map<Uint32, std::function<void(SDL_Event &e)>> callbacks;

const SDL_Color white = {255, 255, 255, 255};

void addCallback(Uint32 key, std::function<void(SDL_Event &e)> val) {
    eventMTX.lock();
    callbacks[key] = val;
    eventMTX.unlock();
}

void eventHandler() {
    SDL_Event e;
    while (SDL_WaitEvent(&e)) {
        eventMTX.lock();
        auto c = callbacks.find(e.type);
        if (c != callbacks.end())
            c->second(e);
        eventMTX.unlock();
    }
            
}

void clearScreen() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void drawText(std::string text, int x, int y, const SDL_Color& color) {
    SDL_Surface *message_surface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), color, WINDOW_WIDTH);
    SDL_Texture *message_texture = SDL_CreateTextureFromSurface(renderer, message_surface);
    SDL_Rect message_rect = {x, y, message_surface->w, message_surface->h};

    SDL_RenderCopy(renderer, message_texture, NULL, &message_rect);
    SDL_FreeSurface(message_surface);
    SDL_DestroyTexture(message_texture);
}

void showMessage(std::string m) {
    clearScreen();
    drawText(m, 0, 0, white);
    SDL_RenderPresent(renderer);
}

void init() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    window = SDL_CreateWindow(
        "CSynth",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP
    );

    SDL_SetWindowSize(window, WINDOW_WIDTH, WINDOW_HEIGHT);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);

    TTF_Init();
    atexit(quit);
    font = TTF_OpenFont("fonts/default.ttf", FONT_SIZE);
    if (font == NULL) {
        std::cerr << "font error" << std::endl;
        exit(EXIT_FAILURE);
    }

    callbacks[SDL_QUIT] = [](SDL_Event &e){ exit(EXIT_SUCCESS); };

    new std::thread(eventHandler);

    clearScreen();
    drawText("Welcome to CSynth!", 0, 0, white);
    SDL_RenderPresent(renderer);
    SDL_Delay(1000);
}

void quit() {
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void showError(std::string err) {
    clearScreen();
    drawText(err, 0, 0, (SDL_Color){255, 0, 0, 255});
    SDL_RenderPresent(renderer);
    SDL_Delay(3000);
}

int choose(std::string prompt, std::vector<std::string> choices) {
    std::string text = prompt+"\n";
    for (int i = 0; i < choices.size(); ++i) {
        text += std::to_string(i) + ") " + choices[i] + "\n";
    }
    clearScreen();
    drawText(text, 0, 0, white);
    SDL_RenderPresent(renderer);

    int selection = 0;
    int n_choices = choices.size();
    bool active = true;
    std::mutex selectionMTX;

    selectionMTX.lock();

    eventMTX.lock();
    callbacks[SDL_KEYDOWN] = [&selectionMTX, &selection, n_choices, &active](SDL_Event& e) {
        if (!active) return;
        char code = e.key.keysym.sym - '0';
        if (code >= 0 && code < n_choices) {
            selection = code;
            active = false;
            selectionMTX.unlock();
        }
    };
    eventMTX.unlock();

    // should block until callback unlocks it
    selectionMTX.lock();
    selectionMTX.unlock();

    eventMTX.lock();
    callbacks.erase(SDL_KEYDOWN);
    eventMTX.unlock();

    return selection;
}

}