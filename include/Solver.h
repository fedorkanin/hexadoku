#pragma once

#include <stdbool.h>

#include "Node.h"

/// @brief Mark constraint as covered.
/// @param column_header The column header of the corresponding constraint.
///
/// @details Cover iterates over all rows that can no longer be used, because
/// they contain a node in the already satisfied column. It detaches all the
/// nodes in such rows from their vertical neighbors and decrements the node
/// count of their respective column header.
void  cover(Node* column_header);

/// @brief Uncover constraint, i.e. make it available again (undo cover).
/// @param column_header The column header of the corresponding constraint.
///
/// @details Uncover iterates over all rows that can be used again, because they
/// contain a node in the uncovered column. It reattaches all the nodes in such
/// rows to their vertical neighbors and increments the node count of their
/// respective column header.
void  uncover(Node* column_header);

/// @brief  Get the first column header with the fewest node count.
Node* getMinColumn(Node* head);

/// @brief  Link neighboring nodes' pointers with each other to bypass node.
/// @param  node The node to bypass.
void  unlinkHorizontal(Node* node);
/// @brief  Point relevant left and right pointers of neighbors to node.
/// @param  node The node to link with.

void  relinkHorizontal(Node* node);

/// @brief  Link neighboring nodes' pointers with each other to bypass node.
/// @param  node The node to bypass.
void  unlinkVertical(Node* node);

/// @brief  Point relevant up and down pointers of neighbors to node.
/// @param  node The node to link with.
void  relinkVertical(Node* node);
