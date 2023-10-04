#pragma once

#include <math.h>
#include <stdbool.h>
#include <stdint.h>

#include "BoolVector2D.h"
#include "Constants.h"
#include "IntVector.h"

int           rowInExactCoverMatrix(int row, int column, int num);

int           sudokuValueFromExactCoverRow(int index);
int           sudokuColumnFromExactCoverRow(int index);
int           sudokuRowFromExactCoverRow(int index);

int           createCellConstraints(BoolVector2D* exact_cover, int header);
int           createRowConstraints(BoolVector2D* exact_cover, int header);
int           createColumnConstraints(BoolVector2D* exact_cover, int header);
int           createBoxConstraints(BoolVector2D* exact_cover, int header);

BoolVector2D* hexadokuToExactCover(uint8_t** hexadoku);
bool          isExactCoverValid(BoolVector2D* exact_cover);
