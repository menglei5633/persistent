//
// Created by lm on 18-9-1.
//
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
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
        this->hashFunctions[i] = HashFunctionFactory::createBKDRHashFunction(dimission, bitSize);
    }
}

void BloomFilter::add(int *data) {
    int i;
    for (i = 0; i < this->k; ++i) {
        int a = this->hashFunctions[i]->hash(data);
        printf("a   ::%d \n", a);
        this->bitArray->set_to_1(this->hashFunctions[i]->hash(data));
    }
}

void BloomFilter::add(char *data) {
    int i;
    for (i = 0; i < this->k; ++i) {
        int a = this->hashFunctions[i]->hash(data);
        printf("a:%d \n", a);
        this->bitArray->set_to_1(this->hashFunctions[i]->hash(data));
    }
}

bool BloomFilter::is_inserted(int *data) {
    int i;
    for (i = 0; i < this->k; ++i) {
//        int a = this->hashFunctions[i].hash(data);
//        printf("%d \n", a);
        if (!this->bitArray->is_1(this->hashFunctions[i]->hash(data))) {
            return false;
        }
    }
    return true;
}

bool BloomFilter::is_inserted(char *data) {
    int i;
    for (i = 0; i < this->k; ++i) {
//        int a = this->hashFunctions[i].hash(data);
//        printf("%d \n", a);
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

BloomFilter::~BloomFilter() {
    free(this->bitArray);
    int i;
    for (i = 0; i < this->k; ++i) {
        free (this->hashFunctions[i]);
    }
    free(this->hashFunctions);
}