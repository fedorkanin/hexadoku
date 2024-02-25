#pragma once

#include <stddef.h>
#include <stdio.h>

#define SUDOKU_SIZE 16
#define BOX_SIZE 4

#ifdef DEBUG
#define DEBUG_PRINTF(...)    \
    do {                     \
        printf(__VA_ARGS__); \
    } while (false)
#else
#define DEBUG_PRINTF(...) \
    do {                  \
    } while (false)
#endif

extern const size_t LINE_WIDTH;
extern const size_t LINE_HEIGHT;