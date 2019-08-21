//
// Created by Daddy on 8/08/2019.
//

#include <stdlib.h>
#include <string.h>
#include "save.h"
#include "personalization.h"


Savefile *new_savefile(char *saveName) {
    Savefile *savefile = malloc(sizeof(Savefile));
    savefile->file = fopen(saveName, "r");

    return savefile;
}

int can_parse_first_two(Savefile *savefile) {
    char line[BUFFER_SIZE];
    fgets(line, BUFFER_SIZE, savefile->file);
    int counter = 0;
    for (int i = 0; i < strlen(line); ++i) {
        counter = (line[i] == ' ') ? counter + 1 : counter;
        if (counter > 3) {
            return 0;
        }
    }

    char *check1, *check2, *check3, *check4;
    int w = (int) strtol(line, ref(check1), 10);
    if (check1[0] != ' ' || w > 100) { return 0; }

    int h = (int) strtol(check1, ref(check2), 10);
    if (check2[0] != ' ' || h > 100) { return 0; }

    int n = (int) strtol(check2, ref(check3), 10);
    if (check3[0] != ' ') { return 0; }

    int v = (int) strtol(check3, ref(check4), 10);
    if (check4[0] != '\n' _or v < 1 _or v > 2) { return 0; }

    savefile->w = w;
    savefile->h = h;
    savefile->n = n;
    savefile->v = v;

    savefile->deckName = malloc(sizeof(char) * BUFFER_SIZE);
    fgets(line, BUFFER_SIZE, savefile->file);
    line[strlen(line) - 1] = '\0';
    memcpy(savefile->deckName, line, BUFFER_SIZE);
    return 1;
}



