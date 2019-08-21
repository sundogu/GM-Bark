//
// Created by Daddy on 3/08/2019.
//

#ifndef BARKPROJECT_PLAYER_H
#define BARKPROJECT_PLAYER_H

#include "personalization.h"
#include "deck.h"

#define HUMAN 'h'
#define AI 'a'


typedef struct {
    char type;
    Deck *_hand;
    int turn;
} Player;


// Function prototype //
/**
 * Create a new player
 * @param type
 * @param turn
 * @return
 */
Player *new_player(char type, int turn);

#endif //BARKPROJECT_PLAYER_H




