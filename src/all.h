#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ncurses.h>
#include <sys/wait.h>
#include <dirent.h>

#include "store/store.h"
#include "ui/ui.h"
#include "ui/modal.h"
#include "steps/steps.h"
#include "steps/locale.h"
#include "steps/disk.h"
#include "steps/confirm.h"
#include "install/install.h"
