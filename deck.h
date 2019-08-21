//
// Created by Daddy on 3/08/2019.
//

#ifndef BARKPROJECT_DECK_H
#define BARKPROJECT_DECK_H

#include <stdio.h>
#include "bin.h"

#define WC '*'

typedef struct {
    char suit;
    int num;
} Card;

typedef struct {
    Card *_list;
    int capacity, size, dealt;
} Deck;


// Function Prototype //
/**
 * Create a new deck
 * @param capacity
 * @return
 */
Deck *new_deck(int capacity);

/**
 * Check and parse line into a Deck. Returns NULL if parsing failed
 * @param line
 * @return
 */
Deck *parse_deck(char *line, int turn, Bin* bin);

/**
 * Add card into deck
 * @param deck
 * @param card
 */
void add_card(Deck *deck, Card card, Bin *bin);

/**
 * Increase size of deck
 * @param deck
 */
void get_bigger_deck(Deck *deck, Bin *bin);

/**
 * Add card to the deck in reverse order
 * @param deck
 * @param card
 */
void add_card_bottom_up(Deck *deck, Card card);

/**
 * Remove the LAST card from the deck
 * @param deck
 * @return
 */
Card next_card(Deck *deck);

/**
 * Burn through the deck up to num.
 * @param deck
 * @param num
 */
void burn_cards(Deck *deck, int num);

/**
 * Print the deck to stdout
 * @param deck
 */
void display_hand(Deck *deck, FILE *file);

/**
 * Get the nth card in deck
 * @param order
 * @return
 */
Card get_card(Deck *deck, int order);

/**
 * Check if deck is exhausted
 * @param deck
 * @return
 */
int deck_empty(Deck *deck);

/**
 * Return the max point in the deck. If empty deck, returns 0
 * @param deck
 * @return
 */
int max_point(Deck *deck);

/**
 * Create new card
 * @param suit
 * @param num
 * @return
 */
Card new_card(char suit, int num);

#endif //BARKPROJECT_DECK_H
