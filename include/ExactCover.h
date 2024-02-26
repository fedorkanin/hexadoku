#pragma once

#include <math.h>
#include <stdbool.h>
#include <stdint.h>

#include "BoolVector2D.h"
#include "Constants.h"

/// @brief Horizontal index in the exact cover matrix representing the
/// constraint (row, column, num).
/// @param row Row in sudoku.
/// @param column Column in sudoku.
/// @param num Number in sudoku.
/// @return Horizontal index of the constraint.
int  indexInExactCoverMatrix(int row, int column, int num);

/// @brief Get the number in the cell that the constraint represents.
int  numFromExactCoverIndex(int index);
/// @brief Get the column in the cell that the constraint represents.
int  columnFromExactCoverIndex(int index);
/// @brief Get the row in the cell that the constraint represents.
int  rowFromExactCoverIndex(int index);

/// @brief Create cell constraints: each cell must contain exactly one number.
/// @param exact_cover The exact cover matrix, initialized with false before
/// creating constraints.
/// @param header The header to start from. It represents the horizontal index
/// of current constraint.
/// @return The new header.
int  createCellConstraints(BoolVector2D* exact_cover, int header);

/// @brief Create row constraints: each number must appear exactly once in each
/// row. For more information, see createCellConstraints.
int  createRowConstraints(BoolVector2D* exact_cover, int header);

/// @brief  Create column constraints: each number must appear exactly once in
/// each column. For more information, see createCellConstraints.
int  createColumnConstraints(BoolVector2D* exact_cover, int header);

/// @brief Create box constraints: each number must appear exactly once in each
/// box. For more information, see createCellConstraints.
int  createBoxConstraints(BoolVector2D* exact_cover, int header);
/// @brief Helper function for createBoxConstraints.
void markBoxConstraint(BoolVector2D* exact_cover, int startRow, int startColumn,
                       int num, int header);

BoolVector2D* hexadokuToExactCover(uint8_t** hexadoku);
bool          isExactCoverValid(BoolVector2D* exact_cover);
