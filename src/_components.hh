#pragma once

// C standard libraries
#include <stdint.h>
#include <unistd.h>

// C++ standard libraries
#include <string>
#include <vector>

// C libraries
#include <ncurses.h>

// types
typedef uint16_t input_t;

// macros
#define CTRL(x) ((x) & 0x1f)
