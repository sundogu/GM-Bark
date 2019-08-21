//
// Created by Daddy on 3/08/2019.
//

#include <stdlib.h>
#include <stdio.h>
#include "error.h"
#include "personalization.h"


ErrorCode process_error_code(ErrorCode err) {
    switch (err) {
        case NUM_ARGS:
            fprintf(stderr,
                    "%s %s\n", "Usage: bark savefile p1type p2type",
                    "bark deck width height p1type p2type");
            break;
        case ARG_TYPE:
            fprintf(stderr, "Incorrect arg types\n");
            break;
        case DECK_FILE:
            fprintf(stderr, "Unable to parse deckfile\n");
            break;
        case SAVE_FILE:
            fprintf(stderr, "Unable to parse savefile\n");
            break;
        case SHORT_DECK:
            fprintf(stderr, "Short deck\n");
            break;
        case BOARD_FULL:
            fprintf(stderr, "Board full\n");
            break;
        case EOI:
            fprintf(stderr, "\nEnd of input\n");
            break;
        case OK:
            // Its empty here
            break;
    }

    return err;
}

