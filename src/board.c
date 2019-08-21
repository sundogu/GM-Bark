//
// Created by Daddy on 5/08/2019.
//

#include <stdlib.h>
#include <stdio.h>
#include "board.h"
#include "personalization.h"


Board *new_board(int w, int h) {
    Board *board = malloc(sizeof(Board));
    board->width = w;
    board->height = h;
    board->num = 0;
    board->_list = malloc(sizeof(Card *) * w);

    for (int i = 0; i < w; board->_list[i++] = malloc(sizeof(Card) * h));
    fill_sentinel(board);

    return board;
}

void fill_sentinel(Board *board) {
    for (int i = 0; i < board->width; ++i) {
        for (int j = 0; j < board->height; ++j) {
            board->_list[i][j] = new_card(WC, 0);
        }
    }
}

void display_board(Board *board, FILE *file) {
    for (int i = 0; i < board->width; ++i) {
        for (int j = 0; j < board->height; ++j) {
            Card c = board->_list[i][j];
            if (c.suit == WC _and file == stdout) {
                fprintf(file, "..");
            } else if (c.suit == WC) {
                fprintf(file, "**");
            }
            else {
                fprintf(file, "%d%c", c.num, c.suit);
            };
        }

        fprintf(file, "\n");
    }
}

void play_card(Board *board, Card card, int col, int row) {
    board->_list[row - 1][col - 1] = card; // convert to index first!!
    board->num++;
}

int board_full(Board *board) {
    return (board->num == board->height * board->width) ? 1 : 0;
}

int board_empty(Board *board) {
    return (board->num == 0) ? 1 : 0;
}

int can_play(Board *board, int i, int j) {
    return (board->_list[i][j].suit != WC) ? 0 : has_adjacent(board, i, j);
}

int has_adjacent(Board *board, int i, int j) {
    int t, l, d, r; // too long don't read.
    t = (j - 1 == -1) ? board->height - 1 : j - 1;
    l = (i - 1 == -1) ? board->width - 1 : i - 1;
    d = (j + 1 == board->height) ? 0 : j + 1;
    r = (i + 1 == board->width) ? 0 : i + 1;

    return (board->_list[i][t].suit != WC _or board->_list[l][j].suit != WC
            _or board->_list[i][d].suit != WC _or board->_list[r][j].suit != WC)
           ? 1 : 0;
}