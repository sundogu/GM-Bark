//
// Created by Daddy on 3/08/2019.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "deck.h"
#include "personalization.h"


Deck *new_deck(int capacity) {
    Deck *deck = malloc(sizeof(Deck));
    deck->capacity = capacity;
    deck->size = 0;
    deck->dealt = 0;
    deck->_list = malloc(sizeof(Card) * deck->capacity);

    return deck;
}

Deck *parse_deck(char *line, int turn, Bin* bin) {
    line[strlen(line) - 1] = '\0';
    if ((turn == 1 _and strlen(line) != 12)
        _or (turn == 0 _and strlen(line) != 10)) {
        return NULL;
    }

    int toggle = 0;
    for (int i = 0; i < strlen(line); ++i) {
        if (toggle _xor 1) {
            if ((!isdigit(line[i])) _or atoi(ref(line[i])) == 0) {
                return NULL;
            }
        } else {
            if (!isupper(line[i])) {
                return NULL;
            }
        }
    }

    Deck *deck = new_deck(BUFFER_SIZE);
    trash_this_ptr(bin, deck);
    trash_this_ptr(bin, deck->_list);

    for (int i = 0; i < strlen(line); i += 2) {
        add_card(deck, new_card(line[i + 1], atoi(ref(line[i]))), bin);
    }

    return deck;
}

void add_card(Deck *deck, Card card, Bin *bin) {
    if (deck->size == deck->capacity) {
        get_bigger_deck(deck, bin);
    } else {
        deck->_list[deck->size++] = card;
    }
}

void add_card_bottom_up(Deck *deck, Card card) {
    if (deck->size == 0) {
        deck->size = deck->capacity;
    }

    deck->_list[--(deck->size)] = card;
}

void get_bigger_deck(Deck *deck, Bin *bin) {
    deck->capacity *= 2;
    int index = 0;

    for (int i = 0; i < bin->size; ++i) {
        index = (bin->_list[i] == deck->_list) ? i : index;
    }
    deck->_list = realloc(deck->_list, sizeof(Card) * deck->capacity);
    trash_ptr_at_i(bin, deck->_list, index);
}

Card next_card(Deck *deck) {
    deck->dealt++;
    return deck->_list[--(deck->capacity)];
}

void burn_cards(Deck *deck, int num) {
    for (int i = 0; i < num - 1; ++i) {
        next_card(deck);
    }
}

void display_hand(Deck *deck, FILE *file) {
    for (int i = 0; i < deck->size; ++i) {
        Card c = deck->_list[i];
        if (c.suit != WC _and file == stdout) {
            fprintf(file, " %d%c", c.num, c.suit);
        } else if (c.suit != WC) {
            fprintf(file, "%d%c", c.num, c.suit);
        }
    }

    fprintf(file, "\n");
}

Card get_card(Deck *deck, int order) {
    int count = 1;
    for (int i = 0; i < deck->size; ++i) {
        if (deck->_list[i].suit != WC) {
            if (count++ == order) {
                Card c = new_card(deck->_list[i].suit, deck->_list[i].num);
                deck->_list[i].suit = WC;
                return c;
            }
        }
    }

    // Should be unreachable code
    return new_card(WC, 0);
}

int deck_empty(Deck *deck) {
    return deck->capacity == 0 ? 1 : 0;
}

int max_point(Deck *deck) {
    int max = 0;
    for (int i = 0; i < deck->size; ++i) {
        max = (deck->_list[i].num > max) ? deck->_list[i].num : max;
    }

    return max;
}

Card new_card(char suit, int num) {
    return (Card) {.suit = suit, .num = num};
}



