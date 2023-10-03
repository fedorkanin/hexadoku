#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "Constants.h"
#include "Coords.h"
#include "Node.h"

Node* createMonkeyFistMesh(uint8_t** cell_matrix);
void  printMonkeyFistMesh(Node* head);
void  validateMonkeyFistMesh(Node* head);
void  freeMonkeyFistMesh(Node* head);
