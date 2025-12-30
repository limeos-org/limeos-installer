/**
 * This code is responsible for displaying a confirmation summary
 * of all user selections before proceeding with installation.
 */

#include "../all.h"

int run_confirmation_step(WINDOW *modal)
{
    Store *store = get_store();

    // Clear and draw step header.
    clear_modal(modal);
    wattron(modal, A_BOLD | COLOR_PAIR(1));
    mvwprintw(modal, 2, 3, "Step 3: Confirm Installation");
    wattroff(modal, A_BOLD);

    // Display summary of selected options.
    mvwprintw(modal, 4, 3, "Ready to install LimeOS with the following settings:");
    mvwprintw(modal, 6, 3, "  Locale: %s", store->locale);
    mvwprintw(modal, 7, 3, "  Disk:   %s", store->disk);

    // Display warning about disk formatting.
    wattron(modal, A_BOLD);
    mvwprintw(modal, 9, 3, "WARNING: All data on %s will be erased!", store->disk);
    wattroff(modal, A_BOLD);

    // Display navigation footer.
    mvwprintw(modal, MODAL_HEIGHT - 2, 3,
              "Press Enter to install, or 'q' to quit.");
    wrefresh(modal);

    // Wait for user confirmation or quit.
    int key;
    while ((key = wgetch(modal)) != '\n' && key != 'q') {
        // Ignore other input.
    }

    return key == '\n';
}
