#pragma once
#include "./all.h"

/**
 * Represents a single installation step.
 */
typedef struct {
    const char *name;
    const char **content;
    int content_lines;
    const char *footer;
} Step;

/**
 * Represents a selectable option in a step.
 */
typedef struct {
    char value[128];
    char label[256];
} StepOption;

/**
 * Maximum number of options in a selection list.
 */
#define STEPS_MAX_OPTIONS 32

/**
 * Displays a step in the modal window.
 *
 * @param modal The modal window to draw in.
 * @param step_number The current step number (1-indexed).
 * @param step The step data to display.
 */
void display_step(WINDOW *modal, int step_number, Step *step);

/**
 * Waits for user input to proceed or quit.
 *
 * @param modal The modal window to capture input from.
 *
 * @return 1 if user pressed Enter to proceed, 0 if user pressed 'q' to quit.
 */
int await_step_input(WINDOW *modal);

/**
 * Renders a selection list with scrolling support.
 *
 * @param modal The modal window to draw in.
 * @param options Array of options to display.
 * @param count Number of options in the array.
 * @param selected Index of the currently selected option.
 * @param start_y Y position to start rendering options.
 * @param scroll_offset Index of the first visible option.
 * @param max_visible Maximum number of options to display at once.
 */
void render_step_options(WINDOW *modal, StepOption *options, int count, int selected,
                         int start_y, int scroll_offset, int max_visible);

/**
 * Runs an interactive selection step, returning the selected index.
 *
 * @param modal The modal window to draw in.
 * @param title The step title to display.
 * @param step_number The current step number (1-indexed).
 * @param description Text shown above the options.
 * @param options Array of options to choose from.
 * @param count Number of options in the array.
 * @param selected Pointer to store the selected index, also used as initial selection.
 *
 * @return 1 if user confirmed selection, 0 if user quit.
 */
int run_selection_step(WINDOW *modal, const char *title, int step_number,
                       const char *description, StepOption *options, int count, int *selected);
