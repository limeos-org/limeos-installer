/**
 * This code is responsible for executing the installation process, including
 * disk formatting, rootfs extraction, and bootloader installation.
 */

#include "../all.h"

static int format_disk(const char *disk)
{
    // TODO: Implement actual disk formatting logic.
    (void)disk;
    napms(500);
    return 0;
}

static int extract_rootfs(const char *disk)
{
    // TEMP: Remove when actual disk target is used.
    // This line is just to avoid unused parameter warning.
    (void)disk;

    // Ensure the placeholder rootfs archive exists.
    if(access("assets/placeholder-rootfs.gz", F_OK) != 0){
        return 1;
    }

    // Extract the rootfs archive to /mnt (the target mount point).
    int cmd = system("tar -xzf assets/placeholder-rootfs.gz -C /mnt &> /dev/null");
    if(cmd == -1){
        // Extraction failed.
        return 1;
    }
    if(WIFEXITED(cmd)){
        int exit_code = WEXITSTATUS(cmd);
        if(exit_code != 0){
            // Extraction failed.
            return 1;
        }
    }

    // Set up necessary mounts for chroot environment.
    cmd = system("sh src/install/mounts.sh &> /dev/null");
    if(cmd == -1){
        // Mount setup failed.
        return 1;
    }
    if(WIFEXITED(cmd)){
        int exit_code = WEXITSTATUS(cmd);
        if(exit_code != 0){
            // Mount setup failed.
            return 1;
        }
    }

    // Extraction and mount setup succeeded.
    return 0;
}

static int setup_bootloader(const char *disk)
{
    // TODO: Implement actual bootloader installation logic.
    (void)disk;
    napms(500);
    return 0;
}

static int configure_locale(const char *locale)
{
    // TODO: Implement locale configuration logic.
    (void)locale;
    napms(200);
    return 0;
}

int run_install(WINDOW *modal)
{
    Store *store = get_store();

    // Display installation progress header.
    clear_modal(modal);
    mvwprintw(modal, 2, 3, "Installing LimeOS...");
    wrefresh(modal);

    // Step 1: Format disk and create partitions.
    mvwprintw(modal, 4, 3, "Formatting %s...", store->disk);
    wrefresh(modal);
    if (format_disk(store->disk) != 0) {
        mvwprintw(modal, 4, 3, "Formatting %s... FAILED", store->disk);
        wrefresh(modal);
        return INSTALL_ERROR_DISK;
    }
    mvwprintw(modal, 4, 3, "Formatting %s... OK", store->disk);
    wrefresh(modal);

    // Step 2: Extract rootfs archive to target.
    mvwprintw(modal, 5, 3, "Extracting system files...");
    wrefresh(modal);
    if (extract_rootfs(store->disk) != 0) {
        mvwprintw(modal, 5, 3, "Extracting system files... FAILED");
        wrefresh(modal);
        return INSTALL_ERROR_EXTRACT;
    }
    mvwprintw(modal, 5, 3, "Extracting system files... OK");
    wrefresh(modal);

    // Step 3: Install and configure bootloader.
    mvwprintw(modal, 6, 3, "Installing bootloader...");
    wrefresh(modal);
    if (setup_bootloader(store->disk) != 0) {
        mvwprintw(modal, 6, 3, "Installing bootloader... FAILED");
        wrefresh(modal);
        return INSTALL_ERROR_BOOTLOADER;
    }
    mvwprintw(modal, 6, 3, "Installing bootloader... OK");
    wrefresh(modal);

    // Step 4: Configure system locale.
    mvwprintw(modal, 7, 3, "Configuring locale...");
    wrefresh(modal);
    configure_locale(store->locale);
    mvwprintw(modal, 7, 3, "Configuring locale... OK");
    wrefresh(modal);

    // Display completion message.
    mvwprintw(modal, 9, 3, "Installation complete!");
    mvwprintw(modal, 10, 3, "Press Enter to exit...");
    wrefresh(modal);

    return INSTALL_SUCCESS;
}
