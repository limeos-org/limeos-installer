#include "../all.h"

static int get_step_row(InstallStep step)
{
    switch (step)
    {
        case STEP_PARTITIONS: return 4;
        case STEP_ROOTFS:     return 5;
        case STEP_BOOTLOADER: return 6;
        case STEP_LOCALE:     return 7;
        default:              return 4;
    }
}

void ncurses_install_progress(
    InstallEvent event, InstallStep step,
    int error_code, void *context
)
{
    WINDOW *modal = (WINDOW *)context;
    if (!modal) return;

    Store *store = get_store();
    int row = get_step_row(step);

    // Decide what to print / display based on the event type.
    switch (event)
    {
        case INSTALL_START:
            clear_modal(modal);
            mvwprintw(modal, 2, 3, "Installing LimeOS...");
            break;

        case INSTALL_STEP_BEGIN:
            if (step == STEP_PARTITIONS)
            {
                mvwprintw(
                    modal, row, 3, "%s %s...",
                    get_install_step_name(step), store->disk
                );
            }
            else
            {
                mvwprintw(modal, row, 3, "%s...", get_install_step_name(step));
            }
            break;

        case INSTALL_STEP_OK:
            if (step == STEP_PARTITIONS)
            {
                mvwprintw(
                    modal, row, 3, "%s %s... OK",
                    get_install_step_name(step), store->disk
                );
            }
            else
            {
                mvwprintw(
                    modal, row, 3, "%s... OK",
                    get_install_step_name(step)
                );
            }
            break;

        case INSTALL_STEP_FAIL:
            if (step == STEP_PARTITIONS)
            {
                mvwprintw(
                    modal, row, 3, "%s %s... FAILED (%d)",
                    get_install_step_name(step), store->disk, error_code
                );
            }
            else
            {
                mvwprintw(
                    modal, row, 3, "%s... FAILED (%d)",
                    get_install_step_name(step), error_code
                );
            }
            break;

        case INSTALL_COMPLETE:
            mvwprintw(modal, 9, 3, "Installation complete!");
            break;

        case INSTALL_AWAIT_REBOOT:
            mvwprintw(modal, 10, 3, "Press Enter to reboot...");
            wrefresh(modal);
            while (wgetch(modal) != '\n');
            return;
    }

    wrefresh(modal);
}
