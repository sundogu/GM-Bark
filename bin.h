//
// Created by Daddy on 3/08/2019.
//

#ifndef BARKPROJECT_BIN_H
#define BARKPROJECT_BIN_H


typedef struct {
    void **_list;
    int capacity, size;
} Bin;

// Function Prototype //
/**
 * Create a new Bin
 * @return the Bin
 */
Bin *new_bin(void);

/**
 * Put pointer into bin.
 * @param bin
 * @param ptr
 */
void trash_this_ptr(Bin *bin, void *ptr);

/**
 * Set index i to ptr
 * @param bin
 * @param ptr
 * @param i
 */
void trash_ptr_at_i(Bin *bin, void *ptr, int i);

/**
 * Increase the size of the bin
 * @param bin
 */
void get_bigger_bin(Bin *bin);

/**
 * Free all dynamically allocated memories collected in the bin
 * @param bin
 */
void take_out_the_trash(Bin **bin);

#endif //BARKPROJECT_BIN_H
