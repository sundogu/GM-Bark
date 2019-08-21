//
// Created by Daddy on 6/08/2019.
//

#include <stdlib.h>
#include <values.h>

#include "score.h"
#include "deck.h"


Node *new_node(int i, int j, char suit) {
    Node *node = malloc(sizeof(Node));
    node->i = i;
    node->j = j;
    node->numCh = 0;
    node->suit = suit;
    node->_children = malloc(sizeof(Node *) * 4);

    return node;
}

void add_child(Node *parent, Node *child) {
    parent->_children[parent->numCh++] = (struct Node *)child;
}

void add_parent(Node *child, Node *parent) {
    child->parent = (struct Node *)parent;
}

int is_child(Node *node, Board *board, int i, int j) {
    return (board->_list[i][j].num > board->_list[node->i][node->j].num) ? 1
                                                                         : 0;
}

int get_long_path(Node *n, char suit) { // One of my proudest functions :')

    if (n->numCh) {
        int pathLen[n->numCh];
        for (int i = 0; i < n->numCh; ++i) {
            pathLen[i] = get_long_path((Node *)n->_children[i], suit);
        }

        int len = max(pathLen, n->numCh);
        if (len) {
            return 1 + len;
        } else {
            return (n->suit == suit) ? 1 : 0;
        }
    } else {
        return (n->suit == suit) ? 1 : 0;
    }
}

int max(int *arr, int len) {
    int max = MININT;
    for (int i = 0; i < len; ++i) {
        max = (arr[i] > max) ? arr[i] : max;
    }

    return max;
}


NodeBoard *new_node_board(int w, int h) {
    NodeBoard *nodeBoard = malloc(sizeof(NodeBoard));
    nodeBoard->width = w;
    nodeBoard->height = h;
    nodeBoard->_list = malloc(sizeof(Node **) * w);

    for (int i = 0; i < w; nodeBoard->_list[i++] = malloc(sizeof(Node *) * h));

    return nodeBoard;
}

void add_node(NodeBoard *nodeBoard, Node *node, int i, int j) {
    nodeBoard->_list[i][j] = node;
}