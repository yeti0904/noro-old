#pragma once

// C standard libraries
#include <stdint.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>

// C++ standard libraries
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

// C libraries
#include <ncurses.h>

// C++ libraries
#include "../inicxx/inicxx.hh"

// types
typedef int16_t input_t;

// macros
#define CTRL(x) ((x) & 0x1f)
#define CTRL_LEFT  546
#define CTRL_RIGHT 561
#define CTRL_DOWN  526
#define CTRL_UP    567
#define CTRL_SLEFT  547
#define CTRL_SRIGHT 562
#define CTRL_SDOWN  527
#define CTRL_SUP    568

#define KEY_SUP      337
#define KEY_SDOWN    336
#define KEY_SHIFTTAB 353

#define COLOR_GREY          8
#define COLOR_BRIGHTRED     9
#define COLOR_BRIGHTGREEN   10
#define COLOR_BRIGHTYELLOW  11
#define COLOR_BRIGHTBLUE    12
#define COLOR_BRIGHTMAGENTA 13
#define COLOR_BRIGHTCYAN    14
#define COLOR_BRIGHTWHITE   15
