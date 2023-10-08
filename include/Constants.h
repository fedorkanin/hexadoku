#pragma once

#include <stddef.h>
#include <stdio.h>

#define SUDOKU_SIZE 9
#define BOX_SIZE 3
#define SEARCH_SINGLE_SOLUTION

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
