#pragma once
#include <stdbool.h>
#include <stdio.h>

#include "Constants.h"
#include "Coords.h"
#include "Node.h"

Node* createMonkeyFistMesh3(uint8_t** hexadoku);
void  printMonkeyFistMesh(Node* head);
void  validateMonkeyFistMesh(Node* head);
void  freeMonkeyFistMesh(Node* head);
