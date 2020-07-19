#include "gui.hpp"

#include <stdlib.h>
#include <iostream>
#include <ncurses.h>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <mutex>
#include <thread>
#include <map>

namespace gui {

TTF_Font *font = NULL;
SDL_Renderer *renderer = NULL;
SDL_Window *window = NULL;
std::mutex mtx;
std::map<Uint32, void (*)(SDL_Event &e)> callbacks;


void eventHandler() {
    SDL_Event e;
    while (true)
        while (SDL_PollEvent(&e)) {
            mtx.lock();
            auto c = callbacks.find(e.type);
            if (c != callbacks.end())
                c->second(e);
            mtx.unlock();
        }
            
}

void clearScreen() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void drawText(std::string text, int x, int y, SDL_Color color) {
    SDL_Surface *message_surface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), color, 640);
    SDL_Texture *message_texture = SDL_CreateTextureFromSurface(renderer, message_surface);
    SDL_Rect message_rect = {x, y, message_surface->w, message_surface->h};

    SDL_RenderCopy(renderer, message_texture, NULL, &message_rect);
    SDL_FreeSurface(message_surface);
    SDL_DestroyTexture(message_texture);
}

void init() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    window = SDL_CreateWindow(
        "CSynth",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640,
        480,
        SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP
    );

    SDL_SetWindowSize(window, 640, 480);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(renderer, 640, 480);

    TTF_Init();
    atexit(quit);
    font = TTF_OpenFont("fonts/default.ttf", 24);
    if (font == NULL) {
        std::cerr << "font error" << std::endl;
        exit(EXIT_FAILURE);
    }

    callbacks[SDL_QUIT] = [](SDL_Event &e){ exit(EXIT_SUCCESS); };

    new std::thread(eventHandler);


    showError("Error della bellissima ragazza che sta leggendo questo!");

    SDL_Delay(3000);
    quit();
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
}

int choose(std::string prompt, std::vector<std::string> choices) {
    mvprintw(0, 0, (prompt+"\n").c_str());
    for (int i = 0; i < choices.size(); ++i) {
        printw("%d) %s\n", i, choices[i].c_str());
    }
    printw("> ");
    int choice = getch() - '0';
    while (choice < 0 || choice >= choices.size()) choice = getch() - '0';

    return choice;
}

}