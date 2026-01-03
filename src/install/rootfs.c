#include "../all.h"

int extract_rootfs(void)
{
    Store *store = get_store();

    // In non-dry-run mode, ensure the rootfs archive exists.
    if (!store->dry_run)
    {
        if (access("assets/placeholder-rootfs.gz", F_OK) != 0)
        {
            return -1;
        }
    }

    // Extract the rootfs archive to /mnt.
    if (run_command("tar -xzf assets/placeholder-rootfs.gz -C /mnt 2>/dev/null") != 0)
    {
        return -2;
    }

    // Set up necessary mounts for chroot environment.
    if (run_command("sh src/install/mounts.sh 2>/dev/null") != 0)
    {
        return -3;
    }

    return 0;
}
