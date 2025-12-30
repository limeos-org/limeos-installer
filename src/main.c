/**
 * This code is responsible for the main entry point of the installation
 * wizard. It initializes the UI, runs through each installation step, and
 * executes the installation process.
 */

#include "all.h"

int main(void)
{
    init_ui();

    // Create the centered modal window for wizard content.
    WINDOW *modal = create_modal("Installation Wizard");

    // Run locale selection step.
    if (!run_locale_step(modal)) {
        destroy_modal(modal);
        cleanup_ui();
        return 0;
    }

    // Run disk selection step.
    if (!run_disk_step(modal)) {
        destroy_modal(modal);
        cleanup_ui();
        return 0;
    }

    // Run confirmation step.
    if (!run_confirmation_step(modal)) {
        destroy_modal(modal);
        cleanup_ui();
        return 0;
    }

    // Run installation using settings from global state.
    int result = run_install(modal);

    // Wait for final input before exiting.
    await_step_input(modal);

    // Cleanup ncurses resources before exit.
    destroy_modal(modal);
    cleanup_ui();

    return result;
}
