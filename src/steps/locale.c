/**
 * This code is responsible for detecting available system locales
 * and presenting them for user selection.
 */

#include "../all.h"

static int get_locale_priority(const char *locale)
{
    // Priority 1: en_US is the most common default.
    if (strncmp(locale, "en_US", 5) == 0) {
        return 1;
    }

    // Priority 2: Other English locales.
    if (strncmp(locale, "en_", 3) == 0) {
        return 2;
    }

    // Priority 3: Major European locales.
    if (strncmp(locale, "de_", 3) == 0 ||
        strncmp(locale, "fr_", 3) == 0 ||
        strncmp(locale, "es_", 3) == 0 ||
        strncmp(locale, "it_", 3) == 0 ||
        strncmp(locale, "pt_", 3) == 0 ||
        strncmp(locale, "nl_", 3) == 0) {
        return 3;
    }

    // Priority 4: All other locales.
    return 4;
}

static int compare_locales(const void *a, const void *b)
{
    const StepOption *option_a = (const StepOption *)a;
    const StepOption *option_b = (const StepOption *)b;

    int priority_a = get_locale_priority(option_a->value);
    int priority_b = get_locale_priority(option_b->value);

    // Sort by priority first.
    if (priority_a != priority_b) {
        return priority_a - priority_b;
    }

    // Then alphabetically within same priority.
    return strcmp(option_a->value, option_b->value);
}

static int is_technical_locale(const char *locale)
{
    // Filter out C locale variants.
    if (strncmp(locale, "C.", 2) == 0 || strcmp(locale, "C") == 0) {
        return 1;
    }

    // Filter out POSIX locale.
    if (strcmp(locale, "POSIX") == 0) {
        return 1;
    }

    return 0;
}

int detect_locales(StepOption *options, int max_count)
{
    // Run locale -a to get available system locales.
    FILE *pipe = popen("locale -a 2>/dev/null", "r");
    if (pipe == NULL) {
        // Fallback to a default locale if detection fails.
        snprintf(options[0].value, sizeof(options[0].value), "en_US.UTF-8");
        snprintf(options[0].label, sizeof(options[0].label), "en_US.UTF-8 (Default)");
        return 1;
    }

    int count = 0;
    char line[256];

    while (fgets(line, sizeof(line), pipe) != NULL && count < max_count) {
        // Remove trailing newline from locale name.
        line[strcspn(line, "\n")] = '\0';

        // Skip empty lines.
        if (strlen(line) == 0) {
            continue;
        }

        // Skip non-UTF-8 locales for cleaner list.
        if (strstr(line, "UTF-8") == NULL && strstr(line, "utf8") == NULL) {
            continue;
        }

        // Skip technical locales like C.utf8 and POSIX.
        if (is_technical_locale(line)) {
            continue;
        }

        snprintf(options[count].value, sizeof(options[count].value), "%s", line);
        snprintf(options[count].label, sizeof(options[count].label), "%s", line);
        count++;
    }

    pclose(pipe);

    // Ensure at least one fallback option exists.
    if (count == 0) {
        snprintf(options[0].value, sizeof(options[0].value), "en_US.UTF-8");
        snprintf(options[0].label, sizeof(options[0].label), "en_US.UTF-8 (Default)");
        return 1;
    }

    // Sort locales by priority then alphabetically.
    qsort(options, count, sizeof(StepOption), compare_locales);

    return count;
}

int run_locale_step(WINDOW *modal)
{
    StepOption options[STEPS_MAX_OPTIONS];
    int count = detect_locales(options, STEPS_MAX_OPTIONS);

    // en_US.UTF-8 should now be first after sorting.
    int selected = 0;

    int result = run_selection_step(modal, "Locale", 1,
                                    "Select your system locale:",
                                    options, count, &selected);

    if (result) {
        // Store the selected locale in global store.
        Store *store = get_store();
        snprintf(store->locale, sizeof(store->locale), "%s", options[selected].value);
    }

    return result;
}
