#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "Constants.h"
#include "InputFunctions.h"
#include "stdio.h"
#include "stdlib.h"

/// @brief Checks if the given 16x16 hexadoku puzzle is valid.
///
/// This function checks that each row, column and box of the sudoku puzzle
/// contains at most one unique number. 0 represents an empty cell in the
/// puzzle.
///
/// @param hexadoku A pointer to a 16x16 array representing the hexadoku puzzle.
/// @return true if the hexadoku puzzle is valid, false otherwise.
bool isHexadokuValid(uint8_t** hexadoku);

/// @brief Prints the given 16x16 hexadoku puzzle to the standard output.
/// @param hexadoku A pointer to a 16x16 array representing the hexadoku puzzle
/// to be printed.
void printHexadoku(uint8_t** hexadoku);
