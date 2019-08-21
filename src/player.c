//
// Created by Daddy on 4/08/2019.
//

#include <stdlib.h>
#include "player.h"


Player *new_player(char type, int turn) {
    Player *player = malloc(sizeof(Player));
    player->_hand = new_deck(100);
    player->type = type;
    player->turn = turn;

    return player;
}