//
// Created by Daddy on 3/08/2019.
//

#ifndef BARKPROJECT_BOARD_H
#define BARKPROJECT_BOARD_H

#include <stdio.h>
#include "deck.h"

#define MIN_DIM 3
#define MAX_DIM 100


typedef struct {
    int width, height, num;
    Card **_list;
} Board;


// Function Prototype //
/**
 * Create a new board of width x height
 * @param width
 * @param height
 * @return
 */
Board *new_board(int width, int height);

/**
 * Fill the board with sentinel cards ('*', '0')
 * @param board
 */
void fill_sentinel(Board *board);

/**
 * Print the board to stdout
 * @param board
 */
void display_board(Board *board, FILE *file);

/**
 * Play the card onto the board
 * @param board
 * @param card
 * @param row
 * @param col
 */
void play_card(Board *board, Card card, int col, int row);

/**
 * Check if board is full
 * @param board
 * @return
 */
int board_full(Board *board);

/**
 * Check if board is empty
 * @param board
 * @return
 */
int board_empty(Board *board);


int can_play(Board *board, int i, int j);

int has_adjacent(Board *board, int i, int j);

#endif //BARKPROJECT_BOARD_H
