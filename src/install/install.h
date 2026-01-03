#pragma once
#include "../all.h"

/**
 * Runs the full installation process using settings from the global store.
 *
 * @param modal The modal window for displaying progress.
 *
 * @return - `0` - on success.
 * @return - `-1` - if partitioning fails.
 * @return - `-2` - if rootfs extraction fails.
 * @return - `-3` - if bootloader setup fails.
 */
int run_install(WINDOW *modal);
