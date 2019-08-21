//
// Created by Daddy on 3/08/2019.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "game.h"
#include "player.h"
#include "personalization.h"


Game *new_game(char **argv) {
    Game *game = malloc(sizeof(Game));
    game->argv = argv;
    game->err = OK;
    game->_bin = new_bin();
    trash_this_ptr(game->_bin, game);

    return game;
}


ErrorCode initial_status_check(Game *game, int argc) {
    if (argc != 4 _and argc != 6) {
        game->err = NUM_ARGS;
        return game->err;
    }

    return (argc == 4) ? init_four_args(game) : init_six_args(game);
}

ErrorCode init_four_args(Game *game) {
    char **argv = game->argv;
    if (!((argv[2][0] == HUMAN _or argv[2][0] == AI)
          _and (argv[3][0] == HUMAN _or argv[3][0] == AI))) {
        game->err = ARG_TYPE;
    } else if (!can_open_file(argv[1], "r")) {
        game->err = SAVE_FILE;
    }

    return game->err;
}

ErrorCode init_six_args(Game *game) {
    char **argv = game->argv;
    char *leftover;

    if (!((argv[4][0] == HUMAN _or argv[4][0] == AI)
          _and (argv[5][0] == HUMAN _or argv[5][0] == AI)
          _and strtol(argv[2], ref(leftover), 10) >= MIN_DIM
          _and strtol(argv[2], ref(leftover), 10) <= MAX_DIM
          _and strcmp(leftover, "") == 0
          _and strtol(argv[3], ref(leftover), 10) >= MIN_DIM
          _and strtol(argv[3], ref(leftover), 10) <= MAX_DIM
          _and strcmp(leftover, "") == 0)) {
        game->err = ARG_TYPE;
    } else if (!can_open_file(argv[1], "r")) {
        game->err = DECK_FILE;
    }

    return game->err;
}

int can_open_file(char *fileName, char *mode) {
    FILE *file = fopen(fileName, mode);
    if (file == NULL) {
        return 0;
    }

    fclose(file);
    return 1;
}

int start_new_game(Game *game) {
    parse_deck_file(game);
    if (game->err != OK) {
        return game->err;
    }

    setup_players(game, 1);
    deal_cards(game);
    setup_board(game);
    play_game(game);

    if (game->err != OK) {
        return game->err;
    }

    calculate_score(game);
    return game->err;
}

int continue_game(Game *game) {
    parse_save_file(game);
    if (game->err != OK) {
        return game->err;
    }

    if (board_full(game->_board)) {
        game->err = BOARD_FULL;
        return game->err;
    }

    play_game(game);

    if (game->err != OK) {
        return game->err;
    }

    calculate_score(game);
    return game->err;
}

int parse_save_file(Game *game) {
    game->savefile = new_savefile(game->argv[1]);
    trash_this_ptr(game->_bin, game->savefile);

    if (!can_parse_first_two(game->savefile)) {
        game->err = SAVE_FILE;
        return game->err;
    }

    trash_this_ptr(game->_bin, game->savefile->deckName);

    if (valid_saved_args(game) != 0 _or !valid_saved_hands(game)
        _or !valid_saved_board(game)) {
        game->err = SAVE_FILE;
        return game->err;
    }

    parse_deck_file(game);
    if (game->err != OK _or game->_deck->capacity < game->savefile->n) {
        game->err = SAVE_FILE;
        return game->err;
    }

    burn_cards(game->_deck, game->savefile->n);
    Player *p = (game->_player1->turn) ? game->_player1 : game->_player2;
    Card c = next_card(game->_deck);
    if (p->_hand->_list[5].suit != c.suit _or p->_hand->_list[5].num != c.num) {
        game->err = SAVE_FILE;
    }

    p->_hand->_list[5] = new_card(WC, 0);

    fclose(game->savefile->file);

    return game->err;
}

int parse_deck_file(Game *game) {
    FILE *file = fopen(game->argv[1], "r");
    char line[BUFFER_SIZE];
    if (fscanf(file, "%[^\n]", line) == 0 _or strlen(line) == 1) {
        game->err = DECK_FILE;
    } else {
        for (int i = 0; i < strlen(line) - 1; ++i) {
            if (!isdigit(line[i])) {
                game->err = DECK_FILE;
                return game->err;
            }
            if (i == 0 _and line[i] == '0') {
                game->err = DECK_FILE;
                return game->err;
            }
        }

        game->_deck = new_deck(atoi(line));
        trash_this_ptr(game->_bin, game->_deck);
        trash_this_ptr(game->_bin, game->_deck->_list);
        if (game->_deck->capacity < 11) {
            game->err = SHORT_DECK;
            return game->err;
        } else {
            fgets(line, BUFFER_SIZE, file); // burn a newline
            int counter = 0;
            while (fgets(line, BUFFER_SIZE, file) != NULL) {
                if (++counter == game->_deck->capacity + 1
                    _and line[0] == '\n') {
                    break;
                } else if ((!(isupper(line[0]) _and isdigit(line[1])
                              _and atoi(ref(line[1])) > 0 _and line[2] == '\n'))
                           _or counter > game->_deck->capacity) {
                    game->err = DECK_FILE;
                    return game->err;
                } else {
                    add_card_bottom_up(game->_deck,
                                       new_card(line[0], atoi(ref(line[1]))));
                }
            }

            if (counter < game->_deck->capacity) {
                game->err = DECK_FILE;
            }
        }
    }

    fclose(file);
    return game->err;
}

void setup_players(Game *game, int playerTurn) {
    int turn = (playerTurn == 1) ? 1 : 0;
    game->_player1 = new_player(game->argv[4][0], turn);
    game->_player2 = new_player(game->argv[5][0], 1 - turn);

    trash_this_ptr(game->_bin, game->_player1);
    trash_this_ptr(game->_bin, game->_player2);
    trash_this_ptr(game->_bin, game->_player1->_hand);
    trash_this_ptr(game->_bin, game->_player2->_hand);
    trash_this_ptr(game->_bin, game->_player1->_hand->_list);
    trash_this_ptr(game->_bin, game->_player2->_hand->_list);
}

void deal_cards(Game *game) {
    while (game->_player1->_hand->size < 5) {
        add_card(game->_player1->_hand, next_card(game->_deck), game->_bin);
    }

    while (game->_player2->_hand->size < 5) {
        add_card(game->_player2->_hand, next_card(game->_deck), game->_bin);
    }
}

void setup_board(Game *game) {
    game->_board = new_board(atoi(game->argv[3]), atoi(game->argv[2]));

    trash_this_ptr(game->_bin, game->_board);
    trash_this_ptr(game->_bin, game->_board->_list);
    for (int i = 0; i < game->_board->width; ++i) {
        trash_this_ptr(game->_bin, game->_board->_list[i]);
    }
}

void play_game(Game *game) {
    for (_ever) {
        display_board(game->_board, stdout);
        Player *p = (game->_player1->turn) ? game->_player1 : game->_player2;
        int pInt = (p == game->_player1) ? 1 : 2;
        deal_and_display(game, p, pInt);

        if (p->type == HUMAN) {
            scan_input(game, p);
        } else {
            if (pInt == 1) {
                play_ai_one(p, pInt, game->_board);
            } else {
                play_ai_two(p, pInt, game->_board);
            }
        }

        if (game_ended(game)) {
            if (game->err != EOI) {
                display_board(game->_board, stdout);
            }

            break;
        }

        toggle_player_turn(game);
    }
}


void deal_and_display(Game *game, Player *p, int pInt) {
    add_card(p->_hand, next_card(game->_deck), game->_bin);

    if (p->type == HUMAN) {
        printf("Hand(%d):", pInt);
    } else {
        printf("Hand:");
    }

    display_hand(p->_hand, stdout);
}

void scan_input(Game *game, Player *player) {
    char line[BUFFER_SIZE];

    do {
        printf("Move? ");
        fgets(line, BUFFER_SIZE, stdin);
    } while (!valid_action(game, line));

    process_action(game, line, player);
}

void toggle_player_turn(Game *game) {
    game->_player1->turn _xor 1;
    game->_player2->turn _xor 1;
}


int valid_action(Game *game, char *line) {
    if (feof(stdin)) {
        return 1;
    } else if (line[0] != 'S') {
        return valid_card_input_check(game, line);
    } else {
        return valid_save_input_check(game, line);
    }
}

int valid_card_input_check(Game *game, char *line) {
    int counter = 0;
    for (int i = 0; i < strlen(line); ++i) {
        counter = (line[i] == ' ') ? counter + 1 : counter;
        if (counter > 2) { return 0; }
    }

    int card = (int) strtol(line, ref(line), 10);
    if (line[0] != ' ') {
        return 0;
    }

    int col = (int) strtol(line, ref(line), 10);
    if (line[0] != ' ') { return 0; }

    int row = (int) strtol(line, ref(line), 10);
    if (line[0] != '\n') { return 0; }

    if (!(card >= 1 _and card <= 6)
        _or !(row >= 1 _and row <= game->_board->width)
        _or !(col >= 1 _and col <= game->_board->height)) {
        return 0;
    }

    if (!board_empty(game->_board)
        _and !can_play(game->_board, row - 1, col - 1)) {
        return 0;
    }

    return 1;
}

int valid_save_input_check(Game *game, char *line) {
    line[strlen(line) - 1] = '\0';
    if (strlen(line) < 5 _or (line[0] != 'S' _and line[1] != 'A'
                              _and line[2] != 'V' _and line[3] != 'E')) {
        printf("Unable to save\n");
        return 0;
    }

    line += strlen("SAVE");
    if (!can_open_file(line, "w")) {
        printf("Unable to save\n");
        return 0;
    }

    return 1;
}

int valid_saved_args(Game *game) {
    char **oldArgs = game->argv;
    char **newArgs = malloc(sizeof(char *) * 6);
    trash_this_ptr(game->_bin, newArgs);

    newArgs[0] = oldArgs[0];
    newArgs[1] = game->savefile->deckName;
    newArgs[4] = oldArgs[2];
    newArgs[5] = oldArgs[3];

    newArgs[2] = malloc(sizeof(char) * 4);
    newArgs[3] = malloc(sizeof(char) * 4);
    trash_this_ptr(game->_bin, newArgs[2]);
    trash_this_ptr(game->_bin, newArgs[3]);

    sprintf(newArgs[2], "%d", game->savefile->w);
    sprintf(newArgs[3], "%d", game->savefile->h);

    game->argv = newArgs;

    return init_six_args(game);
}

int valid_saved_hands(Game *game) {
    int turn = (game->savefile->v == 1) ? 1 : 0;
    char *line = malloc(sizeof(char) * BUFFER_SIZE);
    trash_this_ptr(game->_bin, line);

    fgets(line, BUFFER_SIZE, game->savefile->file);
    Deck *hand1 = parse_deck(line, turn, game->_bin);
    if (hand1 == NULL) { return 0; }

    turn _xor 1;
    fgets(line, BUFFER_SIZE, game->savefile->file);
    Deck *hand2 = parse_deck(line, turn, game->_bin);
    if (hand2 == NULL) { return 0; }

    game->_player1 = (game->savefile->v == 1) ?
                     new_player(game->argv[4][0], 1) :
                     new_player(game->argv[4][0], 0);
    trash_this_ptr(game->_bin, game->_player1);
    trash_this_ptr(game->_bin, game->_player1->_hand);
    trash_this_ptr(game->_bin, game->_player1->_hand->_list);
    game->_player1->_hand = hand1;

    game->_player2 = (game->savefile->v == 1) ?
                     new_player(game->argv[5][0], 0) :
                     new_player(game->argv[5][0], 1);
    trash_this_ptr(game->_bin, game->_player2);
    trash_this_ptr(game->_bin, game->_player2->_hand);
    trash_this_ptr(game->_bin, game->_player2->_hand->_list);
    game->_player2->_hand = hand2;

    return 1;
}

int valid_saved_board(Game *game) {
    char *line = malloc(sizeof(char) * (game->savefile->w + 1) * 2);
    trash_this_ptr(game->_bin, line);
    setup_board(game);

    int i = 0;
    int counter = 0;
    while (fgets(line, (game->savefile->h + 1) * 2, game->savefile->file)
           != NULL) {
        if (line[game->savefile->w * 2] != '\n' _or ++i > game->savefile->h) {
            return 0;
        }

        line[strlen(line) - 1] = '\0';
        for (int j = 0; j < game->savefile->w * 2; j += 2) {
            if (!(isupper(line[j + 1]) _and isdigit(line[j])
                  _and atoi(ref(line[j])) != 0)
                _and !(line[j] == WC _and line[j + 1] == WC)) {
                return 0;
            } else if (line[j + 1] != WC) {
                counter++;
                play_card(game->_board,
                          new_card(line[j + 1], atoi(ref(line[j]))),
                          i, j / 2 + 1);
            }
        }
    }

    if (i < game->savefile->h _or counter + 11 > game->savefile->n) {
        return 0;
    }

    for (int j = 0; j < game->_board->width; ++j) {
        for (int k = 0; k < game->_board->height; ++k) {
            if (game->_board->_list[k][j].suit != WC
                _and !has_adjacent(game->_board, j, k)
                _and counter > 1) {
                return 0;
            }
        }
    }

    return 1;
}

void process_action(Game *game, char *line, Player *player) {
    if (feof(stdin)) {
        game->err = EOI;
    } else if (line[0] != 'S') {
//        int card, col, row;
//        sscanf(line, "%d, %d, %d", ref(card), ref(col), ref(row));
        int card = atoi(ref(line[0]));
        int col = atoi(ref(line[2]));
        int row = atoi(ref(line[4]));
        play_card(game->_board, get_card(player->_hand, card), col, row);
    } else {
        line += strlen("SAVE");
        save_game(game, line);
        scan_input(game, player);
    }
}

void save_game(Game *game, char *line) {
    FILE *file = fopen(line, "w");
    int turn = (game->_player1->turn) ? 1 : 2;

    fprintf(file, "%d %d %d %d\n", game->_board->width, game->_board->height,
            game->_deck->dealt, turn);
    fprintf(file, "%s\n", game->argv[1]);

    display_hand(game->_player1->_hand, file);
    display_hand(game->_player2->_hand, file);

    display_board(game->_board, file);
}

int play_ai_one(Player *p, int pInt, Board *board) {
    if (board_empty(board)) {
        Card c = get_card(p->_hand, 1);
        play_card(board, c, (board->height + 1) / 2,
                  (board->width + 1) / 2);
        printf("Player %d plays %d%c in column %d row %d\n", pInt, c.num,
               c.suit, (board->width + 1) / 2, (board->height + 1) / 2);
    } else {
        for (int i = 0; i < board->width; ++i) {
            for (int j = 0; j < board->height; ++j) {
                if (can_play(board, i, j)) {
                    Card c = get_card(p->_hand, 1);
                    play_card(board, c, j + 1, i + 1);
                    printf("Player %d plays %d%c in column %d row %d\n", pInt,
                           c.num, c.suit, j + 1, i + 1);

                    return 0;
                }
            }
        }
    }

    return 0;
}

int play_ai_two(Player *p, int pInt, Board *board) {
    if (board_empty(board)) {
        Card c = get_card(p->_hand, 1);
        play_card(board, c, (board->height + 1) / 2,
                  (board->width + 1) / 2);
        printf("Player %d plays %d%c in column %d row %d\n", pInt, c.num,
               c.suit, (board->width + 1) / 2, (board->height + 1) / 2);
    } else {
        for (int i = board->width - 1; i >= 0; --i) {
            for (int j = board->height - 1; j >= 0; --j) {
                if (can_play(board, i, j)) {
                    Card c = get_card(p->_hand, 1);
                    play_card(board, c, j + 1, i + 1);
                    printf("Player %d plays %d%c in column %d row %d\n", pInt,
                           c.num, c.suit, j + 1, i + 1);

                    return 0;
                }
            }
        }
    }

    return 0;
}

int game_ended(Game *game) {
    return (board_full(game->_board) _or deck_empty(game->_deck)
            _or game->err != OK) ? 1 : 0;
}

void calculate_score(Game *game) {
    game->_nodeBoard = new_node_board(game->_board->width,
                                      game->_board->height);

    trash_this_ptr(game->_bin, game->_nodeBoard);
    trash_this_ptr(game->_bin, game->_nodeBoard->_list);
    for (int i = 0; i < game->_nodeBoard->width; ++i) {
        trash_this_ptr(game->_bin, game->_nodeBoard->_list[i]);
    }

    populate_node_board(game);

    Deck *p1 = new_deck(BUFFER_SIZE);
    Deck *p2 = new_deck(BUFFER_SIZE);

    trash_this_ptr(game->_bin, p1);
    trash_this_ptr(game->_bin, p2);
    trash_this_ptr(game->_bin, p1->_list);
    trash_this_ptr(game->_bin, p2->_list);

    find_long_path(game, p1, p2);

    printf("Player 1=%d Player 2=%d\n", max_point(p1), max_point(p2));
}

void populate_node_board(Game *game) {
    for (int i = 0; i < game->_board->width; ++i) {
        for (int j = 0; j < game->_board->height; ++j) {
            Card c = game->_board->_list[i][j];
            if (c.suit != WC) {
                Node *n = new_node(i, j, c.suit);
                trash_this_ptr(game->_bin, n);
                trash_this_ptr(game->_bin, n->_children);

                add_node(game->_nodeBoard, n, i, j);
                find_children(game, n);
            } else {
                game->_nodeBoard->_list[i][j] = NULL;
            }
        }
    }
}

void find_children(Game *game, Node *node) {
    Board *board = game->_board;
    int t, l, d, r; // too long don't read.
    t = (node->j - 1 == -1) ? board->height - 1 : node->j - 1;
    l = (node->i - 1 == -1) ? board->width - 1 : node->i - 1;
    d = (node->j + 1 == board->height) ? 0 : node->j + 1;
    r = (node->i + 1 == board->width) ? 0 : node->i + 1;

    if (is_child(node, board, node->i, t)) {
        Node *n = new_node(node->i, t, game->_board->_list[node->i][t].suit);
        trash_this_ptr(game->_bin, n);
        trash_this_ptr(game->_bin, n->_children);

        add_child(node, n);
    }

    if (is_child(node, board, l, node->j)) {
        Node *n = new_node(l, node->j, game->_board->_list[l][node->j].suit);
        trash_this_ptr(game->_bin, n);
        trash_this_ptr(game->_bin, n->_children);

        add_child(node, n);
    }

    if (is_child(node, board, node->i, d)) {
        Node *n = new_node(node->i, d, game->_board->_list[node->i][d].suit);
        trash_this_ptr(game->_bin, n);
        trash_this_ptr(game->_bin, n->_children);

        add_child(node, n);
    }

    if (is_child(node, board, r, node->j)) {
        Node *n = new_node(r, node->j, game->_board->_list[r][node->j].suit);
        trash_this_ptr(game->_bin, n);
        trash_this_ptr(game->_bin, n->_children);

        add_child(node, n);
    }

    for (int i = 0; i < node->numCh; ++i) {
        add_parent((Node *) node->_children[i], node);
        find_children(game, (Node *) node->_children[i]);
    }
}

void find_long_path(Game *game, Deck *p1, Deck *p2) {
    for (int i = 0; i < game->_nodeBoard->width; ++i) {
        for (int j = 0; j < game->_nodeBoard->height; ++j) {
            if (game->_nodeBoard->_list[i][j] != NULL) {
                Node *n = game->_nodeBoard->_list[i][j];
                if (n->suit % 2) {
                    add_card(p1, new_card(n->suit, get_long_path(n, n->suit)),
                             game->_bin);
                } else {
                    add_card(p2, new_card(n->suit, get_long_path(n, n->suit)),
                             game->_bin);
                }
            }
        }
    }
}



