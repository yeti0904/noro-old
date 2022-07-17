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

// C libraries
#include <ncurses.h>

// C++ libraries
#include "../inicxx/inicxx.hh"

// types
typedef int16_t input_t;

// macros
#define CTRL(x) ((x) & 0x1f)
#define COLOR_GREY          8
#define COLOR_BRIGHTRED     9
#define COLOR_BRIGHTGREEN   10
#define COLOR_BRIGHTYELLOW  11
#define COLOR_BRIGHTBLUE    12
#define COLOR_BRIGHTMAGENTA 13
#define COLOR_BRIGHTCYAN    14
#define COLOR_BRIGHTWHITE   15
