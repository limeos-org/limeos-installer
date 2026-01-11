#include "../all.h"

int setup_bootloader(void)
{
    Store *store = get_store();
    const char *disk = store->disk;

    // Detect UEFI by checking if /sys/firmware/efi exists.
    int is_uefi = (access("/sys/firmware/efi", F_OK) == 0);

    // Find the ESP partition index for mounting if in UEFI mode.
    int esp_partition_index = -1;
    if (is_uefi)
    {
        for (int i = 0; i < store->partition_count; i++)
        {
            if (store->partitions[i].flag_esp)
            {
                esp_partition_index = i + 1; // Partitions are 1-indexed.
                break;
            }
        }
    }

    // Mount EFI partition if running in UEFI mode.
    if (is_uefi && esp_partition_index > 0)
    {
        // Construct the ESP partition device path.
        char esp_device[128];
        get_partition_device(disk, esp_partition_index, esp_device, sizeof(esp_device));

        // Create the EFI mount point directory.
        if (run_command("mkdir -p /mnt/boot/efi") != 0)
        {
            return -1; // Failed to create EFI mount point
        }

        // Mount the EFI system partition.
        char mount_cmd[256];
        snprintf(mount_cmd, sizeof(mount_cmd),
            "mount -t vfat %s /mnt/boot/efi", esp_device);
        if (run_command(mount_cmd) != 0)
        {
            return -2; // Failed to mount EFI partition
        }
    }

    // Bind mount system directories needed for chroot environment.
    if (run_command("mount --bind /dev /mnt/dev") != 0)
    {
        return -3; // Failed to bind mount `/dev`.
    }
    if (run_command("mount -t proc proc /mnt/proc") != 0)
    {
        // Rollback previous mount.
        run_command("umount /mnt/dev");
        return -4; // Failed to mount `/proc`.
    }
    if (run_command("mount -t sysfs sys /mnt/sys") != 0)
    {
        // Rollback previous mounts.
        run_command("umount /mnt/proc");
        run_command("umount /mnt/dev");
        return -5; // Failed to mount `/sys`.
    }

    // Install the appropriate GRUB packages based on firmware type.
    // Copy all debs to chroot, install together, then remove.
    char cmd[512];
    const char *pkg_dir = is_uefi
        ? "/usr/share/limeos/packages/efi"
        : "/usr/share/limeos/packages/bios";

    snprintf(cmd, sizeof(cmd), "cp %s/*.deb /mnt/tmp/", pkg_dir);
    if (run_command(cmd) != 0)
    {
        return -6; // Failed to copy GRUB packages
    }

    if (run_command("chroot /mnt sh -c 'dpkg -i /tmp/*.deb' >/dev/null 2>&1") != 0)
    {
        run_command("rm -f /mnt/tmp/*.deb");
        return -7; // Failed to install GRUB packages
    }
    run_command("rm -f /mnt/tmp/*.deb");

    // Install GRUB bootloader based on firmware type.
    if (is_uefi)
    {
        snprintf(cmd, sizeof(cmd),
            "chroot /mnt /usr/sbin/grub-install "
            "--target=x86_64-efi --efi-directory=/boot/efi --bootloader-id=GRUB "
            ">/dev/null 2>&1");
        if (run_command(cmd) != 0)
        {
            return -8; // Failed to run grub-install (UEFI)
        }
    }
    else
    {
        snprintf(cmd, sizeof(cmd),
            "chroot /mnt /usr/sbin/grub-install %s >/dev/null 2>&1", disk);
        if (run_command(cmd) != 0)
        {
            return -9; // Failed to run grub-install (BIOS)
        }
    }

    // Generate GRUB configuration file.
    if (run_command("chroot /mnt /usr/sbin/update-grub >/dev/null 2>&1") != 0)
    {
        return -10; // Failed to run update-grub
    }

    return 0;
}
