/**
 * This code is responsible for managing the global installer state,
 * including user selections and installation progress.
 */

#include "../all.h"

static Store store = {
    .current_step = 0,
    .locale = "",
    .disk = ""
};

Store *get_store(void)
{
    return &store;
}

void reset_store(void)
{
    store.current_step = 0;
    store.locale[0] = '\0';
    store.disk[0] = '\0';
}
