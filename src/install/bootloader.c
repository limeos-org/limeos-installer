#include "../all.h"

int setup_bootloader(void)
{
    Store *store = get_store();
    char cmd[1024];

    // Detect if system is using UEFI by checking for EFI firmware directory.
    int is_uefi = access("/sys/firmware/efi", F_OK) == 0;

    // Mount EFI partition if running in UEFI mode.
    if (is_uefi)
    {
        snprintf(cmd, sizeof(cmd), "mount %s /mnt/boot/efi 2>/dev/null",
                 store->disk);
        if (run_cmd(cmd) != 0)
        {
            return 1;
        }
    }

    // Change to the new root directory before chroot.
    if (!store->dry_run)
    {
        if (chdir("/mnt") != 0)
        {
            return 1;
        }

        // Enter the chroot environment.
        if (chroot("/mnt") != 0)
        {
            return 1;
        }

        // Set working directory to root within chroot.
        if (chdir("/") != 0)
        {
            return 1;
        }
    }

    // Install GRUB bootloader based on firmware type.
    if (is_uefi)
    {
        // Install GRUB for UEFI systems.
        if (run_cmd(
            "grub-install --target=x86_64-efi "
            "--efi-directory=/boot/efi --bootloader-id=GRUB "
            "2>/dev/null"
        ) != 0)
        {
            return 1;
        }
    }
    else
    {
        // Install GRUB for BIOS systems.
        snprintf(cmd, sizeof(cmd), "grub-install %s 2>/dev/null", store->disk);
        if (run_cmd(cmd) != 0)
        {
            return 1;
        }
    }

    // Generate GRUB configuration file.
    if (run_cmd("grub-mkconfig -o /boot/grub/grub.cfg 2>/dev/null") != 0)
    {
        return 1;
    }

    return 0;
}
