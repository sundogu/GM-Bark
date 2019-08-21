//
// Created by Daddy on 3/08/2019.
//

#ifndef BARKPROJECT_GAME_H
#define BARKPROJECT_GAME_H

#include "deck.h"
#include "error.h"
#include "bin.h"
#include "player.h"
#include "board.h"
#include "score.h"
#include "save.h"


typedef struct {
    char **argv;
    Board *_board;
    Player *_player1, *_player2;
    Deck *_deck;
    ErrorCode err;
    NodeBoard *_nodeBoard;
    Savefile *savefile;
    Bin *_bin;
} Game;

// Function Prototype //
/**
 * Create a new game
 * @return
 */
Game *new_game(char **argv);

/**
 * Check stati 1 and 2
 * @param game
 * @param argc
 * @param argv
 * @return
 */
ErrorCode initial_status_check(Game *game, int argc);

/**
 * Check status 2 when there are 4 arguments
 * @param game
 * @param argv
 * @return
 */
ErrorCode init_four_args(Game *game);

/**
 * Check status 2 when there are 6 arguments
 * @param game
 * @param argv
 * @return
 */
ErrorCode init_six_args(Game *game);

/**
 * Checks if file can be opened
 * @param fileName
 * @return
 */
int can_open_file(char *fileName, char *mode);

/**
 * Start a new game of Bark
 * @param game
 * @return
 */
int start_new_game(Game *game);

/**
 * Continue from save file of Bark
 * @param game
 */
int continue_game(Game *game);

/**
 * Parses the save file and setup existing game
 * @param game
 * @return
 */
int parse_save_file(Game *game);

/**
 * Parses the deck file
 * @param game
 * @return
 */
int parse_deck_file(Game *game);

/**
 * Create the two players and stuff
 * @param game
 * @param playerTurn
 */
void setup_players(Game *game, int playerTurn);

/**
 * Deal 5 cards to each player
 * @param game
 */
void deal_cards(Game *game);

/**
 * Set up the game board
 * @param game
 */
void setup_board(Game *game);

/**
 * Players take turns to play the game
 * @param game
 */
void play_game(Game *game);

/**
 * Deal a card to the player and display it to stdout
 * @param game
 * @param player
 * @param pInt
 */
void deal_and_display(Game *game, Player *player, int pInt);

/**
 * Process the player's input
 * @param player
 */
void scan_input(Game *game, Player *player);

/**
 * Process input and return whether
 * @param line
 */
int valid_action(Game *game, char *line);

/**
 * Check for valid input in card input case
 * @param game
 * @param line
 * @return
 */
int valid_card_input_check(Game *game, char *line);

/**
 * Check for valid input in save input case
 * @param game
 * @param line
 * @return
 */
int valid_save_input_check(Game *game, char *line);

/**
 * Convert argv to 6 args syntax and check if valid
 * @param game
 * @return
 */
int valid_saved_args(Game *game);

/**
 * Check and parse hands line into player hands
 * @param game
 * @return
 */
int valid_saved_hands(Game *game);

/**
 * Check and parse board into the game
 * @param game
 * @return
 */
int valid_saved_board(Game *game);

/**
 * Process the action made by player
 * @param game
 * @param line
 * @param player
 */
void process_action(Game *game, char *line, Player *player);

/**
 * Write game to file
 * @param game
 * @param line
 */
void save_game(Game *game, char *line);

/**
 * Change to next player's turn
 * @param game
 */
void toggle_player_turn(Game *game);

/**
 * Play Player 1 AI move
 * @param p
 * @param pInt
 * @param board
 */
int play_ai_one(Player *p, int pInt, Board *board);

/**
 * Play Player 2 AI move
 * @param p
 * @param pInt
 * @param board
 */
int play_ai_two(Player *p, int pInt, Board *board);

/**
 * Check for end of game.
 * @param game
 * @return
 */
int game_ended(Game *game);

/**
 * Calculates the score
 * @param game
 */
void calculate_score(Game *game);

/**
 * Add Nodes to the NodeBoard
 * @param game
 */
void populate_node_board(Game *game);

/**
 * Find the adjacent children of Node
 * @param game
 * @param node
 */
void find_children(Game *game, Node *node);

/**
 * Fills p1 and p2 decks with the longest path of each played cards
 * @param game
 * @param p1
 * @param p2
 */
void find_long_path(Game *game, Deck *p1, Deck *p2);

#endif //BARKPROJECT_GAME_H
