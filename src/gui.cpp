#include "gui.hpp"

#include <ncurses.h>

namespace gui {

void init() {
    initscr();
    use_default_colors();
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
    getch();
}

}