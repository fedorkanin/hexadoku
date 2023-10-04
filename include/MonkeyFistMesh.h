#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "Constants.h"
#include "ExactCover.h"
#include "Node.h"
#include "Pregen.h"

Node*  createMonkeyFistMesh(uint8_t** cell_matrix);
void   printMonkeyFistMesh(Node* head);
void   validateMonkeyFistMesh(Node* head);
void   freeMonkeyFistMesh(Node* head);

Node*  createNodeFromData(int row_index, int col_index, Node** column_headers);
void   linkNodesHorizontally(Node* left_node, Node* right_node);
void   linkNodesVertically(Node* upper_node, Node* lower_node);
Node** createColumnHeaders(int mesh_width, Node* head);

void createNodesUsingPregenArray(uint8_t** cell_matrix, Node** column_headers);
void createNodesUsingExactCover(uint8_t** cell_matrix, Node** column_headers);
