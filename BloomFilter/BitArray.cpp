//
// Created by lm on 18-9-1.

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
//#include <string.h>
#include "BitArray.h"

BitArray::BitArray(int bitSize) {
    this->size = bitSize / 8 + 1;
    this->bits = (char*)malloc( this->size * sizeof(char));
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

char* BitArray::getBits() {
    return this->bits;
}

void BitArray::weiYu(char *bits) {
    int i;
    for (i = 0; i < this->size; ++i) {
        this->bits[i] &= bits[i];
    }
}

void BitArray::weiHuo(char *bits) {
    int i;
    for (i = 0; i < this->size; ++i) {
        this->bits[i] |= bits[i];
    }
}

BitArray::~BitArray() {
    free(this->bits);
}
