//
// Created by Daddy on 3/08/2019.
//

#include <stdlib.h>
#include "bin.h"
#include "personalization.h"


Bin *new_bin(void) {
    Bin *bin = malloc(sizeof(Bin));
    bin->capacity = BUFFER_SIZE;
    bin->size = 0;
    bin->_list = malloc(sizeof(void *) * bin->capacity);

    return bin;
}

void trash_this_ptr(Bin *bin, void *ptr) {
    if (bin->size == bin->capacity) {
        get_bigger_bin(bin);
    } else {
        bin->_list[bin->size++] = ptr;
    }
}

void trash_ptr_at_i(Bin *bin, void *ptr, int i) {
    bin->_list[i] = ptr;
}

void get_bigger_bin(Bin *bin) {
    bin->capacity *= 2;
    bin->_list = realloc(bin->_list, sizeof(void *) * bin->capacity);
}

void take_out_the_trash(Bin **bin) {
    Bin *b = deref(bin);
    for (int i = 0; i < b->size; i++) {
        free(b->_list[i]);
    };

    free(b->_list);
    free(b);
}