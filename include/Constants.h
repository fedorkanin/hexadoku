#pragma once

#include <stddef.h>
#include <stdio.h>

#define SUDOKU_SIZE 16
#define BOX_SIZE 4

// 1. Each cell must contain a number.
// 2. Each row must contain each number exactly once.
// 3. Each column must contain each number exactly once.
// 4. Each box must contain each number exactly once.
#define CONSTRAINTS 4

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
