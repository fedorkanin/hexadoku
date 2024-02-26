#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "Constants.h"

/// @brief Read a hexadoku from the standard input in Progtest format (pretty).
/// @return A pointer to a SUDOKU_SIZE x SUDOKU_SIZE array representing the
/// puzzle.
uint8_t** readProgtest(void);

/// @brief Helper function for readProgtest.
bool      isDelimiterStringValid(char* string, bool is_dashed);

/// @brief Helper function for readProgtest.
bool      isLetterValid(char a);

/// @brief Helper function for readProgtest. Converts a row from pretty-printed
/// sudoku to an array of uint8_t with values [0-SUDOKU_SIZE).
uint8_t*  strToUint8t(char* string);

/// @brief Removes trailing spaces.
void      stripString(char* string);

/// @brief Reads string from stdin using getline, removes trailing newline,
/// returns '\0' string in case of failure.
char*     getString(void);
