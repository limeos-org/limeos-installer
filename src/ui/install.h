#pragma once
#include "../all.h"

/**
 * ncurses-based progress callback for installation.
 * Displays progress in a modal window.
 *
 * @param event Progress event type.
 * @param step Installation step identifier.
 * @param error_code Error code for failure events.
 * @param context Must be a valid WINDOW* pointer.
 */
void ncurses_install_progress(
    InstallEvent event,
    InstallStep step,
    int error_code,
    void *context
);
