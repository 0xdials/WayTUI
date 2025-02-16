#include <ncurses.h>
#include "tui.h"

void init_tui() {
    initscr();             // Start ncurses mode
    noecho();              // Don't echo user input
    cbreak();              // Disable line buffering
    keypad(stdscr, TRUE);  // Enable arrow keys

    // Draw some pretty borders
    box(stdscr, 0, 0);
    mvprintw(1, 2, "Welcome to WayTUI");
    refresh();
}

void draw_login_prompt(char *username) {
    mvprintw(3, 2, "Username: ");
    refresh();
    echo();
    getnstr(username, 31);
    noecho();
}

void cleanup_tui() {
    endwin();
}

