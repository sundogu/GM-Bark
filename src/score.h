//
// Created by Daddy on 6/08/2019.
//

#ifndef BARKPROJECT_SCORE_H
#define BARKPROJECT_SCORE_H


#include "board.h"

typedef struct {
    int i, j, numCh;
    char suit;
    struct Node *parent, **_children;
} Node;

typedef struct {
    int width, height;
    Node ***_list;
} NodeBoard;


// Function Prototype //
/**
 * Create new node
 * @param i
 * @param j
 * @param suit
 * @return
 */
Node *new_node(int i, int j, char suit);

/**
 * Add child node to parent node
 * @param parent
 * @param child
 */
void add_child(Node *parent, Node *child);

/**
 * Add parent node to child node
 * @param child
 * @param parent
 */
void add_parent(Node *child, Node *parent);

/**
 * Check if card at index i, j is a child of Node
 * @param node
 * @param board
 * @param i
 * @param j
 */
int is_child(Node *node, Board *board, int i, int j);

/**
 * A recursive function that returns the longest path from Node n to another
 * Node of the same suit.
 * @param n
 * @param suit
 * @return
 */
int get_long_path(Node *n, char suit);

/**
 * Goes through array arr and return the max int in it. If array is empty,
 * returns MININT instead.
 * @param arr
 * @param len
 * @return
 */
int max(int *arr, int len);

/**
 * Create new NodeBoard
 * @param w
 * @param h
 * @return
 */
NodeBoard *new_node_board(int w, int h);

/**
 * Add node to nodeBoard at index i of j
 * @param nodeBoard
 * @param node
 * @param i
 * @param j
 */
void add_node(NodeBoard *nodeBoard, Node *node, int i, int j);

#endif //BARKPROJECT_SCORE_H
