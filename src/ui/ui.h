#pragma once
#include "../all.h"

/**
 * Initializes the ncurses library and configures color pairs.
 */
void init_ui(void);

/**
 * Cleans up ncurses and restores terminal state.
 */
void cleanup_ui(void);
