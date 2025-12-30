/**
 * This code is responsible for detecting available block devices
 * and presenting them for user selection.
 */

#include "../all.h"

static void format_size(unsigned long long bytes, char *buffer, size_t buffer_size)
{
    double size = (double)bytes;

    if (size >= 1e12) {
        snprintf(buffer, buffer_size, "%.1f TB", size / 1e12);
    } else if (size >= 1e9) {
        snprintf(buffer, buffer_size, "%.1f GB", size / 1e9);
    } else if (size >= 1e6) {
        snprintf(buffer, buffer_size, "%.1f MB", size / 1e6);
    } else {
        snprintf(buffer, buffer_size, "%llu B", bytes);
    }
}

static unsigned long long get_disk_size(const char *device)
{
    char path[256];
    snprintf(path, sizeof(path), "/sys/block/%s/size", device);

    FILE *file = fopen(path, "r");
    if (file == NULL) {
        return 0;
    }

    unsigned long long sectors = 0;
    if (fscanf(file, "%llu", &sectors) != 1) {
        sectors = 0;
    }
    fclose(file);

    // Sector size is typically 512 bytes.
    return sectors * 512;
}

static int is_removable(const char *device)
{
    char path[256];
    snprintf(path, sizeof(path), "/sys/block/%s/removable", device);

    FILE *file = fopen(path, "r");
    if (file == NULL) {
        return 0;
    }

    int removable = 0;
    if (fscanf(file, "%d", &removable) != 1) {
        removable = 0;
    }
    fclose(file);

    return removable;
}

int detect_disks(StepOption *options, int max_count)
{
    DIR *dir = opendir("/sys/block");
    if (dir == NULL) {
        // Fallback if `/sys/block` is unavailable.
        snprintf(options[0].value, sizeof(options[0].value), "/dev/sda");
        snprintf(options[0].label, sizeof(options[0].label), "/dev/sda (Unknown size)");
        return 1;
    }

    int count = 0;
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL && count < max_count) {
        const char *name = entry->d_name;

        // Skip virtual and special devices.
        if (strncmp(name, "loop", 4) == 0 ||
            strncmp(name, "ram", 3) == 0 ||
            strncmp(name, "dm-", 3) == 0 ||
            strncmp(name, "sr", 2) == 0 ||
            strncmp(name, "fd", 2) == 0 ||
            strcmp(name, ".") == 0 ||
            strcmp(name, "..") == 0) {
            continue;
        }

        // Get disk size and format label.
        unsigned long long size = get_disk_size(name);
        if (size == 0) {
            continue;
        }

        char size_str[32];
        format_size(size, size_str, sizeof(size_str));

        // Mark removable disks in the label.
        const char *removable_tag = is_removable(name) ? " [Removable]" : "";

        snprintf(options[count].value, sizeof(options[count].value), "/dev/%s", name);
        snprintf(options[count].label, sizeof(options[count].label),
                 "/dev/%s - %s%s", name, size_str, removable_tag);
        count++;
    }

    closedir(dir);

    // Ensure at least one fallback option exists.
    if (count == 0) {
        snprintf(options[0].value, sizeof(options[0].value), "/dev/sda");
        snprintf(options[0].label, sizeof(options[0].label), "/dev/sda (No disks detected)");
        return 1;
    }

    return count;
}

int run_disk_step(WINDOW *modal)
{
    StepOption options[STEPS_MAX_OPTIONS];
    int count = detect_disks(options, STEPS_MAX_OPTIONS);
    int selected = 0;

    int result = run_selection_step(modal, "Disk Selection", 2,
                                    "Select the target disk for installation:",
                                    options, count, &selected);

    if (result) {
        // Store the selected disk in global store.
        Store *store = get_store();
        snprintf(store->disk, sizeof(store->disk), "%s", options[selected].value);
    }

    return result;
}
