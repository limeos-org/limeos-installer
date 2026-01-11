#include "install.h"
#include "partitions.h"
#include "rootfs.h"
#include "bootloader.h"
#include "locale.h"
#include "cleanup.h"
#include "../utils/command.h"

#define NOTIFY(event, step, err) \
    if (progress_cb) progress_cb((event), (step), (err), context)

int run_install(install_progress_cb progress_cb, void *context)
{
    NOTIFY(INSTALL_START, 0, 0);

    // Step 1: Create partitions and format disk.
    NOTIFY(INSTALL_STEP_BEGIN, STEP_PARTITIONS, 0);
    int result = create_partitions();
    if (result != 0)
    {
        NOTIFY(INSTALL_STEP_FAIL, STEP_PARTITIONS, result);
        cleanup_mounts();
        return -1;
    }
    NOTIFY(INSTALL_STEP_OK, STEP_PARTITIONS, 0);

    // Step 2: Extract rootfs archive to target.
    NOTIFY(INSTALL_STEP_BEGIN, STEP_ROOTFS, 0);
    result = extract_rootfs();
    if (result != 0)
    {
        NOTIFY(INSTALL_STEP_FAIL, STEP_ROOTFS, result);
        cleanup_mounts();
        return -2;
    }
    NOTIFY(INSTALL_STEP_OK, STEP_ROOTFS, 0);

    // Step 3: Install and configure bootloader.
    NOTIFY(INSTALL_STEP_BEGIN, STEP_BOOTLOADER, 0);
    result = setup_bootloader();
    if (result != 0)
    {
        NOTIFY(INSTALL_STEP_FAIL, STEP_BOOTLOADER, result);
        cleanup_mounts();
        return -3;
    }
    NOTIFY(INSTALL_STEP_OK, STEP_BOOTLOADER, 0);

    // Step 4: Configure system locale.
    NOTIFY(INSTALL_STEP_BEGIN, STEP_LOCALE, 0);
    result = configure_locale();
    if (result != 0)
    {
        NOTIFY(INSTALL_STEP_FAIL, STEP_LOCALE, result);
        cleanup_mounts();
        return -4;
    }
    NOTIFY(INSTALL_STEP_OK, STEP_LOCALE, 0);

    // Cleanup.
    close_dry_run_log();
    cleanup_mounts();

    NOTIFY(INSTALL_COMPLETE, 0, 0);
    NOTIFY(INSTALL_AWAIT_REBOOT, 0, 0);

    run_command("reboot");

    return 0;
}
