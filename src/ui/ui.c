/**
 * This code is responsible for initializing and cleaning up the ncurses
 * terminal UI library.
 */

#include "../all.h"

void init_ui(void)
{
    // Initialize ncurses screen and disable input buffering.
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    // Configure color pairs if terminal supports colors.
    if (has_colors()) {
        start_color();
        use_default_colors();

        // Define off-white background for a softer appearance.
        if (can_change_color()) {
            init_color(COLOR_WHITE, 900, 900, 880);
        }

        init_pair(1, COLOR_BLACK, COLOR_WHITE);
    }

    // Refresh to apply initial screen state.
    refresh();
}

void cleanup_ui(void)
{
    endwin();
}
