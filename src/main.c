#include <stdio.h>
#include "game.h"
#include "personalization.h"

int main(int argc, char **argv) {
    Game *game = new_game(argv);
    ErrorCode status;

    if ((status = initial_status_check(game, argc)) == OK) {
        if (argc == 6) {
            start_new_game(game);
        } else {
            continue_game(game);
        }
    }

    status = game->err;
    process_error_code(game->err);
    take_out_the_trash(ref(game->_bin));
    return status;
}