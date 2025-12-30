#pragma once
#include "../all.h"

/**
 * Default modal window width.
 */
#define MODAL_WIDTH 60

/**
 * Default modal window height.
 */
#define MODAL_HEIGHT 15

/**
 * Creates a centered modal window with a title and border.
 *
 * @param title The title to display at the top of the modal.
 *
 * @return The created modal window.
 */
WINDOW *create_modal(const char *title);

/**
 * Clears the content area of a modal window, preserving the border.
 *
 * @param modal The modal window to clear.
 */
void clear_modal(WINDOW *modal);

/**
 * Destroys a modal window and frees resources.
 *
 * @param modal The modal window to destroy.
 */
void destroy_modal(WINDOW *modal);
