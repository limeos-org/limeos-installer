#include "../all.h"

int create_partitions(void)
{
    Store *store = get_store();
    const char *disk = store->disk;
    char command[512];

    // Create GPT partition table.
    snprintf(command, sizeof(command), "parted -s %s mklabel gpt 2>/dev/null", disk);
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
            "parted -s %s mkpart %s %lluMiB %lluMiB 2>/dev/null",
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
                "parted -s %s set %d boot on 2>/dev/null",
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
                "parted -s %s set %d esp on 2>/dev/null",
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
            snprintf(command, sizeof(command), "mkfs.ext4 -F %s 2>/dev/null", partition_device);
        }
        else if (partition->filesystem == FS_SWAP)
        {
            snprintf(command, sizeof(command), "mkswap %s 2>/dev/null", partition_device);
        }

        // Execute formatting command.
        if (run_command(command) != 0)
        {
            return -5;
        }
    }

    // Loop over partitions to find root partition and mount it.
    for (int i = 0; i < store->partition_count; i++)
    {
        Partition *partition = &store->partitions[i];
        if (strcmp(partition->mount_point, "/") == 0)
        {
            char partition_device[128];
            get_partition_device(disk, i + 1, partition_device, sizeof(partition_device));
            snprintf(command, sizeof(command), "mount %s /mnt 2>/dev/null", partition_device);
            if (run_command(command) != 0)
            {
                return -6;
            }
            break;
        }
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
            snprintf(command, sizeof(command), "swapon %s 2>/dev/null", partition_device);
            run_command(command);
        }
        else if (strcmp(partition->mount_point, "/") != 0 && partition->mount_point[0] == '/')
        {
            char partition_device[128];
            char mount_path[256];
            get_partition_device(disk, i + 1, partition_device, sizeof(partition_device));
            snprintf(mount_path, sizeof(mount_path), "/mnt%s", partition->mount_point);
            snprintf(
                command, sizeof(command),
                "mkdir -p %s && mount %s %s 2>/dev/null",
                mount_path, partition_device, mount_path
            );
            run_command(command);
        }
    }

    return 0;
}
