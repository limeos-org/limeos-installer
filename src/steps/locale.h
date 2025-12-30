#pragma once
#include "./all.h"

/**
 * Detects available system locales and populates the options array.
 *
 * @param options Array to store detected locales.
 * @param max_count Maximum number of options to detect.
 *
 * @return Number of locales detected.
 */
int detect_locales(StepOption *options, int max_count);

/**
 * Runs the locale selection step interactively.
 * 
 * @param modal The modal window to draw in.
 * 
 * @return 1 if user confirmed selection, 0 if user quit.
 */
int run_locale_step(WINDOW *modal);
