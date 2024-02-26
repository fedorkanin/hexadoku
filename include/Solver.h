#pragma once

#include <stdbool.h>

#include "Node.h"

bool  compareNodeCount(Node* first, Node* second);
void  sortColumnHeaders(Node* head);
void  cover(Node* column_header);
void  uncover(Node* column_header);
Node* getMinColumn(Node* head);

void  unlinkHorizontal(Node* node);
void  relinkHorizontal(Node* node);
void  unlinkVertical(Node* node);
void  relinkVertical(Node* node);
