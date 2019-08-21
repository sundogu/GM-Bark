//
// Created by Daddy on 3/08/2019.
//

#ifndef BARKPROJECT_ERROR_H
#define BARKPROJECT_ERROR_H


typedef enum {
    OK              // 0
    , NUM_ARGS      // 1
    , ARG_TYPE      // 2
    , DECK_FILE     // 3
    , SAVE_FILE     // 4
    , SHORT_DECK    // 5
    , BOARD_FULL    // 6
    , EOI           // 7
} ErrorCode;




// Function Prototypes //
/**
 * Checks the err enum and prints the relevant error message if required to
 * stderr.
 * @param err the ErrorCode enum
 * @return the enum value of err
 */
ErrorCode process_error_code(ErrorCode err);

#endif //BARKPROJECT_ERROR_H
