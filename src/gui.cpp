#include "gui.hpp"

#include <ncurses.h>

namespace gui {

void init() {
    initscr();
    use_default_colors();
    noecho();
    printw("WELCOME TO CSYNTH!\n");
    refresh();
    start_color();
    init_pair(1, COLOR_RED, -1);
}

void clear() {
    endwin();
}

void showError(std::string err) {
    attron(COLOR_PAIR(1));
    printw((err+"\n").c_str());
    attroff(COLOR_PAIR(1));
    refresh();
    getch();
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