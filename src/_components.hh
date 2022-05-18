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

// types
typedef uint16_t input_t;

// macros
#define CTRL(x) ((x) & 0x1f)
