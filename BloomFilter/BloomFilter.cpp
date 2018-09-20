//
// Created by lm on 18-9-1.
//
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BloomFilter.h"

BloomFilter::BloomFilter(int dimission, int k, int bitSize) {
    this->dimission = dimission;
    this->k = k;
    this->bitSize = bitSize;
    this->bitArray = new BitArray(bitSize);

    this->hashFunctions = (HashFunction**)malloc(k * sizeof(HashFunction*));
    assert(this->hashFunctions != NULL);
    int i;
    for (i = 0; i < k; ++i) {
        this->hashFunctions[i] = HashFunctionFactory::createBasicHashFunction(dimission, bitSize);
    }
}

BloomFilter::BloomFilter(const BloomFilter &bl) {
    this->dimission = bl.dimission;
    this->k = bl.k;
    this->bitSize = bl.bitSize;
    this->bitArray = new BitArray(bitSize);

    this->hashFunctions = (HashFunction**)malloc(this->k * sizeof(HashFunction*));
    assert(this->hashFunctions != NULL);
    int i;
    for (i = 0; i < this->k; ++i) {
        this->hashFunctions[i] = HashFunctionFactory::copyBasicHashFunction((BasicHashFunction*)bl.hashFunctions[i]);
    }
}

void BloomFilter::add(int *data) {
    int i;
    for (i = 0; i < this->k; ++i) {
//        int a = this->hashFunctions[i]->hash(data);
//        printf("hashFuncton %d value: %d \n", i, a);
        this->bitArray->set_to_1(this->hashFunctions[i]->hash(data));
    }
}

void BloomFilter::add(char *data) {
    int i;
    for (i = 0; i < this->k; ++i) {
//        int a = this->hashFunctions[i]->hash(data);
//        printf("add hashFuncton %d value: %d \n", i, a);
        this->bitArray->set_to_1(this->hashFunctions[i]->hash(data));
    }
}

bool BloomFilter::is_inserted(int *data) {
    int i;
    for (i = 0; i < this->k; ++i) {
//        int a = this->hashFunctions[i]->hash(data);
//        printf("hashFuncton %d value: %d \n", i, a);
        if (!this->bitArray->is_1(this->hashFunctions[i]->hash(data))) {
            return false;
        }
    }
    return true;
}

bool BloomFilter::is_inserted(char *data) {
    int i;
    for (i = 0; i < this->k; ++i) {
//        int a = this->hashFunctions[i]->hash(data);
//        printf("is_insert hashFuncton %d value: %d \n", i, a);
        if (!this->bitArray->is_1(this->hashFunctions[i]->hash(data))) {
            return false;
        }
    }
    return true;
}

void BloomFilter::all_set_1() {
    this->bitArray->all_set_1();
}

void BloomFilter::all_set_0() {
    this->bitArray->all_set_0();
}

char* BloomFilter::getBits() {
    this->bitArray->getBits();
}

void BloomFilter::printBitArray() {
    int i;
    for (i = 0; i < this->bitSize; ++i) {
        if (this->bitArray->is_1(i)) {
            printf("1 ");
        } else {
            printf("0 ");
        }
    }
    printf("\n");
}

void BloomFilter::weiYu(char *bits) {
    this->bitArray->weiYu(bits);
}

void BloomFilter::weiHuo(char *bits) {
    this->bitArray->weiHuo(bits);
}


BloomFilter::~BloomFilter() {
//    printf("mmmmm\n");
    free(this->bitArray);
    int i;
    for (i = 0; i < this->k; ++i) {
        free (this->hashFunctions[i]);
    }
    free(this->hashFunctions);
//    printf("bloom end\n");
}