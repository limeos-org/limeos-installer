/**
 * This code is responsible for displaying installation steps and handling
 * user input navigation between steps.
 */

#include "../all.h"

void display_step(WINDOW *modal, int step_number, Step *step)
{
    // Clear previous step content before drawing.
    clear_modal(modal);

    // Display step header in bold for emphasis.
    wattron(modal, A_BOLD | COLOR_PAIR(1));
    mvwprintw(modal, 2, 3, "Step %d: %s", step_number, step->name);
    wattroff(modal, A_BOLD);

    // Display each line of step content below header.
    for (int i = 0; i < step->content_lines; i++) {
        mvwprintw(modal, 4 + i, 3, "%s", step->content[i]);
    }

    // Display navigation footer at fixed bottom position.
    if (step->footer != NULL) {
        mvwprintw(modal, MODAL_HEIGHT - 2, 3, "%s", step->footer);
    }

    // Refresh window to show updated content.
    wrefresh(modal);
}

int await_step_input(WINDOW *modal)
{
    int key;
    while ((key = wgetch(modal)) != '\n' && key != 'q') {
        // Ignore scroll and other unhandled input.
    }
    return key == '\n';
}

#define MAX_VISIBLE_OPTIONS (MODAL_HEIGHT - 9)

static void render_scrollbar(WINDOW *modal, int start_y, int track_height,
                             int scroll_offset, int max_visible, int total_count)
{
    // Only show scrollbar if content exceeds visible area.
    if (total_count <= max_visible) {
        return;
    }

    int scrollbar_x = MODAL_WIDTH - 2;

    // Calculate thumb position within the track.
    int max_scroll = total_count - max_visible;
    int thumb_pos = (scroll_offset * (track_height - 1)) / max_scroll;

    // Draw the scrollbar track and thumb using ncurses ACS characters.
    for (int i = 0; i < track_height; i++) {
        if (i == thumb_pos) {
            // Use reverse video space for solid black thumb.
            wattron(modal, A_REVERSE);
            mvwaddch(modal, start_y + i, scrollbar_x, ' ');
            wattroff(modal, A_REVERSE);
        } else {
            mvwaddch(modal, start_y + i, scrollbar_x, ACS_VLINE);
        }
    }
}

void render_step_options(WINDOW *modal, StepOption *options, int count, int selected,
                         int start_y, int scroll_offset, int max_visible)
{
    int visible_count = count < max_visible ? count : max_visible;

    for (int i = 0; i < visible_count; i++) {
        int option_index = scroll_offset + i;

        // Highlight selected option with reverse video.
        if (option_index == selected) {
            wattron(modal, A_REVERSE);
        }

        // Draw selection indicator and option label.
        mvwprintw(modal, start_y + i, 3, "  %s %s",
                  option_index == selected ? ">" : " ",
                  options[option_index].label);

        if (option_index == selected) {
            wattroff(modal, A_REVERSE);
        }
    }

    // Draw scrollbar on the right edge.
    render_scrollbar(modal, start_y, visible_count, scroll_offset, max_visible, count);
}

int run_selection_step(WINDOW *modal, const char *title, int step_number,
                       const char *description, StepOption *options, int count, int *selected)
{
    int current = *selected;
    int scroll_offset = 0;
    int max_visible = MAX_VISIBLE_OPTIONS;

    // Adjust initial scroll to show selected item.
    if (current >= max_visible) {
        scroll_offset = current - max_visible + 1;
    }

    while (1) {
        // Clear and redraw step header.
        clear_modal(modal);
        wattron(modal, A_BOLD | COLOR_PAIR(1));
        mvwprintw(modal, 2, 3, "Step %d: %s", step_number, title);
        wattroff(modal, A_BOLD);

        // Display description text above options.
        mvwprintw(modal, 4, 3, "%s", description);

        // Render the selectable options list with scrolling.
        render_step_options(modal, options, count, current, 6, scroll_offset, max_visible);

        // Display navigation footer.
        mvwprintw(modal, MODAL_HEIGHT - 2, 3,
                  "Arrow keys to select, Enter to confirm, 'q' to quit.");
        wrefresh(modal);

        // Handle navigation input.
        int key = wgetch(modal);
        switch (key) {
            case KEY_UP:
                if (current > 0) {
                    current--;
                    // Scroll up if selection moves above visible area.
                    if (current < scroll_offset) {
                        scroll_offset = current;
                    }
                }
                break;
            case KEY_DOWN:
                if (current < count - 1) {
                    current++;
                    // Scroll down if selection moves below visible area.
                    if (current >= scroll_offset + max_visible) {
                        scroll_offset = current - max_visible + 1;
                    }
                }
                break;
            case '\n':
                *selected = current;
                return 1;
            case 'q':
                return 0;
        }
    }
}
