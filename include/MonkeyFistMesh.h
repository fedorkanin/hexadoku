#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "Constants.h"
#include "Coords.h"
#include "Node.h"

/// @brief Create a DLX mesh from pre-generated coord array, then fill it with
/// hints from the given hexadoku.
/// @param hexadoku A pointer to a SUDOKU_SIZE x SUDOKU_SIZE array.
/// @return A pointer to the head of the DLX mesh.
Node* createDLXMesh(uint8_t** hexadoku);
void  printDLXMesh(Node* head);
void  validateDLXMesh(Node* head);
void  freeDLXMesh(Node* head);
