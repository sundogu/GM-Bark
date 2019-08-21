//
// Created by Daddy on 8/08/2019.
//

#ifndef BARKPROJECT_SAVE_H
#define BARKPROJECT_SAVE_H

#include <stdio.h>
#include "deck.h"
#include "board.h"

typedef struct {
    FILE *file;
    int w, h, n, v;
    char *deckName;
    Deck *hand1, *hand2;
    Board *board;
} Savefile;

// Function Prototype //
/**
 * create new savefile
 * @param saveName
 * @return
 */
Savefile *new_savefile(char *saveName);

/**
 * Try to parse the first two line
 * @param savefile
 * @return
 */
int can_parse_first_two(Savefile *savefile);


#endif //BARKPROJECT_SAVE_H
