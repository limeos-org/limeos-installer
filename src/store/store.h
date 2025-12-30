#pragma once
#include "../all.h"

/**
 * Maximum length for locale string storage.
 */
#define STORE_MAX_LOCALE_LEN 64

/**
 * Maximum length for disk path storage.
 */
#define STORE_MAX_DISK_LEN 64

/**
 * Global store containing user selections and installation settings.
 */
typedef struct {
    int current_step;
    char locale[STORE_MAX_LOCALE_LEN];
    char disk[STORE_MAX_DISK_LEN];
} Store;

/**
 * Retrieves the global store singleton.
 * @return Pointer to the global Store instance.
 */
Store *get_store(void);

/**
 * Resets the global store to default values.
 */
void reset_store(void);
