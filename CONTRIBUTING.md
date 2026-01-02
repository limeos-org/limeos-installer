<picture>
  <!-- The media queries determine the image based on website theme -->
  <source media="(prefers-color-scheme: dark)" srcset=".assets/contributing_banner/dark_mode.png">
  <source media="(prefers-color-scheme: light)" srcset=".assets/contributing_banner/light_mode.png">
  <!-- Fallback to light mode variant if no match -->
  <img alt="LimeOS Banner" src=".assets/contributing_banner/light_mode.png">
</picture>

######

This document outlines the guidelines for contributing to this repository, including best practices for code contributions and other relevant procedures. It is divided into multiple sections, which you can explore in the table of contents below. In the event of any conflicts between these sections, the section listed first will take precedence.

### Table of Contents

**Repository Contributing Guidelines**

- [Building the installation wizard](#building-the-installation-wizard)
- [Running the installation wizard](#running-the-installation-wizard)

**C Language Contributing Guidelines**

- [Writing documentation](#writing-documentation)  
  &nbsp; • &nbsp; [Writing documentation in header files](#writing-documentation-in-header-files)  
  &nbsp; • &nbsp; [Writing documentation in source files](#writing-documentation-in-source-files)
- [Naming code elements and files](#naming-code-elements-and-files)  
  &nbsp; • &nbsp; [Naming functions](#naming-functions)  
  &nbsp; • &nbsp; [Naming variables](#naming-variables)  
  &nbsp; • &nbsp; [Naming parameters](#naming-parameters)  
  &nbsp; • &nbsp; [Naming types](#naming-types)  
  &nbsp; • &nbsp; [Naming macros](#naming-macros)  
  &nbsp; • &nbsp; [Naming files](#naming-files)  
  &nbsp; • &nbsp; [Naming binaries](#naming-binaries)
- [Ordering code declarations](#ordering-code-declarations)
- [Structuring files](#structuring-files)

**General Contributing Guidelines**

- [Understanding the Git workflow](#understanding-the-git-workflow)
- [Determining version numbers](#determining-version-numbers)

&nbsp;

## Repository Contributing Guidelines

### Building the installation wizard

**TBA**

### Running the installation wizard

**TBA**

&nbsp;

## C Language Contributing Guidelines

> **Important:** These guidelines are replicated across all LimeOS repositories that primarily use the C programming language. Any changes made here must also be applied to the `CONTRIBUTING.md` files across similar repositories to maintain consistency.

### Writing documentation

This subsection establishes guidelines for documenting code through comments and Doxygen annotations. Clear documentation reduces onboarding time for new contributors, prevents bugs, and ensures the long-term sustainability of the project.

**Rules**

1. Comment lines **must not** exceed 80 characters in length, including whitespace.

   _**Why?**_ Research suggests ~50-75 characters is easiest to read. 80 is a practical cap that works well in terminals and side-by-side diffs.

2. Comments using sentence grammar **must** follow natural language conventions (e.g., end with a full stop).

   _**Why?**_ Inconsistent punctuation looks sloppy and undermines the professionalism of the codebase.

#### Writing documentation in header files

This subsection defines how to document declarations within header (.h) files using Doxygen format, which allows for automatic documentation generation and standardized presentation.

**Template**

```c
/**
 * [DESCRIPTION].
 *
 * @param [NAME] [PURPOSE].
 *
 * @return [VALUE].
 */
```

Where:

- `DESCRIPTION` - A brief explanation of what the element represents or does.
- `NAME` - The parameter name.
- `PURPOSE` - What the parameter is used for.
- `VALUE` - What the function returns.

**Example**

```c
/**
 * Represents a point in a 2D coordinate system.
 */
typedef struct {
   int x;
   int y;
} Vector2;

/**
 * Computes the linear distance between two Vector2 points
 * in 2D space.
 *
 * @param p1 The first point.
 * @param p2 The second point.
 *
 * @return The calculated distance as a floating-point number.
 */
float calculate_distance(int p1, int p2);

/**
 * Attempts to connect to the server.
 *
 * @param host The server hostname.
 *
 * @return - `0` Indicates successful connection.
 * @return - `-1` Indicates a connection failure.
 * @return - `-2` Indicates an invalid hostname.
 */
int connect_to_server(const char *host);
```

**Rules**

1. Every declaration within a header file **must** be documented using the Doxygen `/** */` format.

   _**Why?**_ Doxygen comments enable automatic documentation generation and provide standardized presentation across tooling.

2. Each section (description, parameters, returns) **must** be separated by a single blank line.

   _**Why?**_ Visual grouping makes the comment structure skimmable at a glance.

3. Functions with discrete return values **should** use the dash notation (`-`) within `@return` to list each possible value.

   _**Why?**_ Listing return codes explicitly prevents callers from having to trace implementation to understand failure modes.

4. Tags **must** be limited to `@param`, `@return`, `@note`, and `@warning`.

   _**Why?**_ These tags are widely supported across tools and editors; exotic tags may not render correctly.

#### Writing documentation in source files

This subsection defines how to document implementation details within source (.c) files using inline comments that explain logic and algorithmic steps.

**Template**

```c
// [VERB:ACTION].
```

Where:

- `VERB` - Indicates the sentence must start with a verb (e.g., "Ensure", "Validate", "Calculate").
- `ACTION` - What the step accomplishes.

**Example**

```c
int update_item_value(Item* item, int new_value)
{
   // Ensure the item pointer is not NULL.
   if (item == NULL) {
      return ERROR_NULL_POINTER;
   }

   // Validate the new value against allowed range.
   if (new_value < MIN_VALUE || new_value > MAX_VALUE) {
      return ERROR_VALUE_OUT_OF_RANGE;
   }

   // Assign the new value to the item.
   item->value = new_value;

   return SUCCESS;
}
```

**Rules**

1. Each logical step within a function **must** have an inline comment above it.

   _**Why?**_ Inline comments let readers skim logic without parsing implementation. Remove the code, and the comments alone should convey the algorithm.

2. Inline comments **must** use `//`, not `/* */` or `/** */`.

   _**Why?**_ Single-line comments signal inline guidance, not API documentation.

3. Inline comments **must** start with a verb (e.g., "Validate...", "Calculate...", "Ensure...").

   _**Why?**_ Verbs describe actions; nouns describe state. Code is action.

4. Complex source files **should** include a multi-line file header comment beginning with "This code is responsible for".

   _**Why?**_ File-level context helps readers understand the module's role before diving into implementation details.

**Example** (file header)

```c
#include <stdio.h>

/**
 * This code is responsible for user authentication and session management.
 * Note that session timestamps use local time instead of UTC, causing
 * potential Daylight Saving Time issues.
 */
```

### Naming code elements and files

This subsection defines naming conventions for functions, variables, types, macros, and files. Unlike many modern programming languages, C lacks an official style guide. In this codebase, we follow naming practices that align closely with traditional C programming patterns found in established projects like the Linux kernel and GNU software.

#### Naming functions

1. Function names **must** follow `snake_case()` convention.

   _**Why?**_ Snake case is the traditional C convention, keeping naming consistent with standard library functions and established C projects.

2. Function names **should** follow a verb-noun structure (e.g., `write_to_buffer()`).

   _**Why?**_ Verb-noun makes intent explicit at the call site, reducing cognitive load when scanning code.

3. Function names **should** incorporate the name of their module (e.g., `read_config()` in `config` module).

   _**Why?**_ Module prefixes prevent name collisions and make the function's origin clear without checking includes.

#### Naming variables

1. Variable names **must** follow `snake_case` convention.

   _**Why?**_ Consistent casing with functions keeps the codebase visually uniform and predictable.

2. Variable names **must** clearly indicate the variable's contents or purpose.

   _**Why?**_ Ambiguous names force readers to trace assignments, breaking their reading flow and forcing unnecessary context-switching.

3. Variable names **must not** use abbreviations, except for standard ones (e.g., `id` for identifier).

   _**Why?**_ Abbreviations create project-specific vocabulary that readers must memorize and reduce discoverability in search.

#### Naming parameters

1. Parameter names **must** follow `snake_case` convention.

   _**Why?**_ Consistent casing with variables keeps function signatures visually uniform.

2. Parameter names **must** clearly indicate the parameter's contents or purpose.

   _**Why?**_ Vague parameter names force readers to jump to the function definition to understand what to pass.

3. Parameter names **must not** use abbreviations, except for standard ones (e.g., `id` for identifier).

   _**Why?**_ Parameters are read most often at the call site. Avoiding abbreviations keeps intent obvious.

4. Output parameters **must** be prefixed with `out_`.

   _**Why?**_ The prefix signals that the function modifies the parameter, preventing accidental misuse.

#### Naming types

1. Type names **must** follow `PascalCase` convention.

   _**Why?**_ PascalCase distinguishes types from variables and functions at a glance.

2. Type names **should** use descriptive nouns or noun phrases (e.g., `UserData`).

   _**Why?**_ Descriptive names reduce cognitive load by making a type's contents clear at a glance.

3. Type names **should** incorporate the name of their module (e.g., `ImageProperties` in `image` module).

   _**Why?**_ Module prefixes prevent name collisions across the codebase.

#### Naming macros

1. Macro names **must** follow `SCREAMING_SNAKE_CASE` convention (all uppercase).

   _**Why?**_ Uppercase signals "compile-time constant" or "macro" at a glance, distinguishing them from runtime values.

2. Macro names **must** clearly indicate the macro's contents or purpose.

   _**Why?**_ Magic values without context force readers to guess intent.

3. Macro names **should** add unit suffixes where applicable (e.g., `_MS`, `_PERCENT`, `_BYTES`).

   _**Why?**_ Unit suffixes prevent unit mismatch bugs and make the value's meaning explicit.

4. Macro names **should** incorporate the name of their module (e.g., `NETWORK_RETRY_COUNT` in `network` module).

   _**Why?**_ Module prefixes prevent name collisions and group related constants visually.

#### Naming files

1. File names **must** follow `snake_case` convention.

   _**Why?**_ Snake case matches the function naming convention and avoids case-sensitivity issues across file systems.

2. File names **should** use concise names, typically 1-2 words.

   _**Why?**_ Short names scan faster in directory trees and include statements.

3. File names **must not** use abbreviations unless widely understood (e.g., `auth`).

   _**Why?**_ Abbreviations make files harder to find via search and increase inconsistent naming.

4. File names **should** let directory structure provide context (e.g., `user/auth/tokens.c`).

   _**Why?**_ Long filenames with multiple concepts signal that directory structure should carry that meaning instead.

#### Naming binaries

1. Binary names **must** follow `dash-case` convention.

   _**Why?**_ Dash case is the standard convention for Unix executables and command-line tools.

2. Binary names **must** start with the `limeos-` prefix (e.g., `limeos-window-manager`).

   _**Why?**_ The prefix identifies the binary as part of the LimeOS project and prevents name collisions with system tools.

3. Binary names **must not** use abbreviations.

   _**Why?**_ Users type binary names directly; full names are more discoverable and memorable.

4. Library binaries **must** append the `-lib` suffix (e.g., `limeos-config-lib`).

   _**Why?**_ The suffix distinguishes libraries from executables at a glance.

### Ordering code declarations

This subsection defines the order in which code elements should appear within source and header files. Consistent ordering helps contributors quickly locate specific elements, reducing cognitive load when navigating unfamiliar code.

Code elements **must** be organized in the following order:

1. **Includes**

   _**Why?**_ Dependencies come first so readers immediately see what the file relies on.

2. **Macros**

   _**Why?**_ Macros must precede code that uses them; placing them early ensures they're defined before use.

3. **Types**

   _**Why?**_ Types must precede variables and functions that reference them.

4. **Global variables**

   _**Why?**_ Variables often depend on types; placing them after ensures dependencies are defined.

5. **Functions**

   _**Why?**_ Logic comes last, after all dependencies (includes, macros, types, data) are established.

### Structuring files

This subsection defines how to organize files and directories within the repository. A well-organized structure enables contributors to quickly locate and understand code components.

**Rules**

1. The root directory **must** contain only essential files: build configurations (`Makefile`), documentation (`README.md`, `CONTRIBUTING.md`), important directories (`src`, `bin`, `obj`), and license information.

   _**Why?**_ A minimal root keeps the entry point clean and scannable; clutter buries important files.

2. All source code **must** reside within the `src` directory.

   _**Why?**_ A single source root makes build configuration simpler and keeps the repository organized.

3. Source code **should** be organized into logical subdirectories by module or feature.

   _**Why?**_ Modular organization groups related code, making navigation intuitive.

4. Source files (`.c`) and header files (`.h`) **must** share the same name and directory.

   _**Why?**_ Paired naming makes finding the interface for any implementation trivial.

5. `main.c` **may** omit a corresponding header file.

   _**Why?**_ Entry points typically don't expose APIs to other modules.

**Example**

```
src/
├── module/
│   ├── feature.c
│   └── feature.h
```

&nbsp;

## General Contributing Guidelines

> **Important:** These guidelines are replicated across all LimeOS repositories. Any changes made here must also be applied to the `CONTRIBUTING.md` files across all other repositories to maintain consistency.

### Understanding the Git workflow

This subsection documents the branching strategy and contribution process. Following this workflow ensures smooth collaboration and maintains code quality across releases.

This repository uses two main branches:

- `main` - Stable release code, must not be pushed to directly.
- `develop` - Development code, must not be pushed to directly.

**Rules**

1. Contributors **must** fork the repository and create a branch from `develop` prefixed with `feature-`.

   _**Why?**_ Feature branches isolate work-in-progress and make code review manageable.

2. Contributors **must not** push directly to `main` or `develop`.

   _**Why?**_ Protected branches ensure all changes go through review, maintaining code quality.

3. Pull requests **must** target the `develop` branch.

   _**Why?**_ Targeting develop allows integration testing before changes reach the stable main branch.

**Example**

```bash
git checkout develop
git checkout -b feature-audio-support
# Make changes and commit
git push origin feature-audio-support
# Submit pull request targeting develop
```

Changes will be reviewed by project maintainers. Approved changes are merged or squashed into `develop`. When sufficient changes accumulate, `develop` is rebased onto `main` and a new release tag is created.

### Determining version numbers

This subsection defines the versioning scheme used for releases. Semantic Versioning provides clear expectations about compatibility between versions.

This repository adheres to Semantic Versioning (SemVer), which uses a three-part version number:

- `MAJOR` - Incremented for incompatible API changes.
- `MINOR` - Incremented for backwards-compatible new features.
- `PATCH` - Incremented for backwards-compatible bug fixes.

**Example**

- `1.0.0` - Initial stable release.
- `1.1.0` - Added new features.
- `1.1.1` - Added bug fixes.
- `2.0.0` - Introduced breaking changes.

A more in-depth guide on SemVer can be found [here](https://semver.org/).
