#include "../all.h"

int create_partitions(void)
{
    Store *store = get_store();
    const char *disk = store->disk;
    char command[512];

    // Create GPT partition table.
    snprintf(command, sizeof(command), "parted -s %s mklabel gpt >/dev/null 2>&1", disk);
    if (run_command(command) != 0)
    {
        return -1;
    }

    // Create each partition in sequence.
    unsigned long long start_mb = 1;
    for (int i = 0; i < store->partition_count; i++)
    {
        Partition *partition = &store->partitions[i];
        unsigned long long size_mb = partition->size_bytes / (1024 * 1024);
        unsigned long long end_mb = start_mb + size_mb;

        // Create partition with calculated boundaries.
        snprintf(
            command, sizeof(command),
            "parted -s %s mkpart %s %lluMiB %lluMiB >/dev/null 2>&1",
            disk,
            (partition->type == PART_PRIMARY) ? "primary" : "logical",
            start_mb, end_mb
        );
        if (run_command(command) != 0)
        {
            return -2;
        }

        // Set boot flag if needed.
        if (partition->flag_boot)
        {
            snprintf(
                command, sizeof(command),
                "parted -s %s set %d boot on >/dev/null 2>&1",
                disk, i + 1
            );
            if (run_command(command) != 0)
            {
                return -3;
            }
        }

        // Set ESP flag if needed.
        if (partition->flag_esp)
        {
            snprintf(
                command, sizeof(command),
                "parted -s %s set %d esp on >/dev/null 2>&1",
                disk, i + 1
            );
            if (run_command(command) != 0)
            {
                return -4;
            }
        }

        // Set BIOS boot flag if needed (for GPT + BIOS boot).
        if (partition->flag_bios_grub)
        {
            snprintf(
                command, sizeof(command),
                "parted -s %s set %d bios_grub on >/dev/null 2>&1",
                disk, i + 1
            );
            if (run_command(command) != 0)
            {
                return -4;
            }
        }

        // Update start for next partition.
        start_mb = end_mb;
    }

    // Format each partition with appropriate filesystem.
    for (int i = 0; i < store->partition_count; i++)
    {
        Partition *partition = &store->partitions[i];
        char partition_device[128];
        get_partition_device(disk, i + 1, partition_device, sizeof(partition_device));

        // Determine formatting command based on filesystem type.
        if (partition->filesystem == FS_EXT4)
        {
            snprintf(command, sizeof(command), "mkfs.ext4 -F %s >/dev/null 2>&1", partition_device);
        }
        else if (partition->filesystem == FS_SWAP)
        {
            snprintf(command, sizeof(command), "mkswap %s >/dev/null 2>&1", partition_device);
        }
        else if (partition->filesystem == FS_FAT32)
        {
            snprintf(command, sizeof(command), "mkfs.vfat -F 32 %s >/dev/null 2>&1", partition_device);
        }
        else
        {
            // Unknown filesystem type, skip formatting.
            continue;
        }

        // Execute formatting command.
        if (run_command(command) != 0)
        {
            return -5;
        }
    }

    // Find the root partition index.
    int root_index = -1;
    for (int i = 0; i < store->partition_count; i++)
    {
        if (strcmp(store->partitions[i].mount_point, "/") == 0)
        {
            root_index = i;
            break;
        }
    }

    // Validate that a root partition exists.
    if (root_index < 0)
    {
        return -6; // No root partition found
    }

    // Mount the root partition.
    char root_device[128];
    get_partition_device(disk, root_index + 1, root_device, sizeof(root_device));
    snprintf(command, sizeof(command), "mount %s /mnt >/dev/null 2>&1", root_device);
    if (run_command(command) != 0)
    {
        return -7; // Failed to mount root partition
    }

    // Mount remaining partitions.
    for (int i = 0; i < store->partition_count; i++)
    {
        Partition *partition = &store->partitions[i];

        // Enable swap or mount at mount point as needed.
        if (partition->filesystem == FS_SWAP)
        {
            char partition_device[128];
            get_partition_device(disk, i + 1, partition_device, sizeof(partition_device));
            snprintf(command, sizeof(command), "swapon %s >/dev/null 2>&1", partition_device);
            if (run_command(command) != 0)
            {
                fprintf(stderr, "Warning: failed to enable swap on %s\n", partition_device);
            }
        }
        else if (strcmp(partition->mount_point, "/") != 0 && partition->mount_point[0] == '/')
        {
            char partition_device[128];
            char mount_path[256];
            get_partition_device(disk, i + 1, partition_device, sizeof(partition_device));
            snprintf(mount_path, sizeof(mount_path), "/mnt%s", partition->mount_point);
            snprintf(
                command, sizeof(command),
                "mkdir -p %s && mount %s %s >/dev/null 2>&1",
                mount_path, partition_device, mount_path
            );
            if (run_command(command) != 0)
            {
                fprintf(stderr, "Warning: failed to mount %s at %s\n", partition_device, mount_path);
            }
        }
    }

    return 0;
}
