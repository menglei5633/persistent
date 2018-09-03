//
// Created by lm on 18-9-1.

#include <assert.h>
#include <stdlib.h>
#include "BitArray.h"

BitArray::BitArray(int bitSize) {
    this->size = bitSize;
    this->bits = (char*)malloc( ((bitSize / 8) + (bitSize % 8 > 0 ? 1 : 0)) * sizeof(char));
    assert(this->bits != NULL);
}

void BitArray::set_to_1(int location) {
    this->bits[location >> SHIFT] |= (1 << (location & MASK));
}

int BitArray::is_1(int location) {
    return this->bits[location >> SHIFT] & (1 << (location & MASK));
}

void BitArray::all_set_1() {
    int i;
    for (i = 0; i < this->size; ++i) {
        this->bits[i] |= 0xFF;
    }
}

void BitArray::all_set_0() {
    int i;
    for (i = 0; i < this->size; ++i) {
        this->bits[i] &= 0x00;
    }
}

BitArray::~BitArray() {
    free(this->bits);
}
