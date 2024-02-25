#pragma once

#include "Node.h"
#include "stdbool.h"

Node* getMiddle(Node* head);
Node* merge(Node* first, Node* second, bool (*comparator)(Node*, Node*));
Node* mergeSort(Node* head, bool (*comparator)(Node*, Node*));
