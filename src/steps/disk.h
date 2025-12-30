#pragma once
#include "./all.h"

/**
 * Detects available block devices and populates the options array.
 *
 * @param options Array to store detected disks.
 * @param max_count Maximum number of options to detect.
 *
 * @return Number of disks detected.
 */
int detect_disks(StepOption *options, int max_count);

/**
 * Runs the disk selection step interactively.
 * 
 * @param modal The modal window to draw in.
 * 
 * @return 1 if user confirmed selection, 0 if user quit.
 */
int run_disk_step(WINDOW *modal);
