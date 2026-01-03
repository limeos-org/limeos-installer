#pragma once
#include "../all.h"

/**
 * Creates partitions, formats them, and mounts them.
 *
 * @return - `0` - on success.
 * @return - `-1` - if partition table creation fails.
 * @return - `-2` - if partition creation fails.
 * @return - `-3` - if boot flag setting fails.
 * @return - `-4` - if ESP flag setting fails.
 * @return - `-5` - if filesystem formatting fails.
 * @return - `-6` - if mounting fails.
 */
int create_partitions(void);
