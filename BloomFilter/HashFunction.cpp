//
// Created by lm on 18-9-1.
//
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "HashFunction.h"

unsigned int BasicHashFunction::hash(int *data) {
    int i;
    unsigned int value = 0;
//    printf("dimission: %d\n", this->dimission);
    for (i = 0; i < this->dimission; ++i) {
//        printf("%d %d\n", this->a[i], data[i]);
        value += this->a[i] * data[i];
    }
//    printf("value: %d\n", value);
    return  value % this->b;
}

unsigned int BasicHashFunction::hash(char *data) {
    unsigned int nLen = strlen((char*)data);
    assert(nLen <= this->dimission);
    unsigned char *p = (unsigned char*)data;
    unsigned int value = 0;
    unsigned int nIndex = 0;

    while (*p) {
        value += this->a[nIndex] * (*p);
        ++p;
        ++nIndex;
    }
    return value % this->b;
}

unsigned int BKDRHashFunction::hash(int *data) {
    unsigned int value = 0;
    int i;
    for (i = 0; i < this->dimission; ++i) {
        value += pow(this->seed, i) * data[i];
    }
    return value % this->b;
}

unsigned int BKDRHashFunction::hash(char *data) {
    unsigned char *p = (unsigned char*)data;
    unsigned int value = 0;
    unsigned int nIndex = 0;
    unsigned int nLen = strlen((char*)p);
    while( *p )
    {
        value += pow(this->seed, nLen-nIndex-1)*(*p);
        ++p;
        nIndex++;
    }
    return value % this->b;
}

HashFunction * HashFunctionFactory::createBasicHashFunction(unsigned int dimission, unsigned int b) {
    assert(b != 0);
    int i;
    BasicHashFunction * bhf = new BasicHashFunction();
    bhf->a = (unsigned int*)malloc(dimission * sizeof(unsigned int));
    bhf->b = b;
    bhf->dimission = dimission;

    for (i = 0; i < dimission; ++i) {
        bhf->a[i] = (unsigned int)HashFunctionFactory::genRandomInt(0, 100);
//        printf("__%d\n", hf.a[i]);
    }
    return bhf;
}

HashFunction * HashFunctionFactory::createBKDRHashFunction(unsigned int dimission, unsigned int b) {
    assert(b != 0);
    int i;
    BKDRHashFunction * bhf = new BKDRHashFunction();
    bhf->dimission = dimission;
    bhf->b = b;

    bhf->seed = HashFunctionFactory::genRandomJiShuInt(1, 100);

    return bhf;


}

int HashFunctionFactory::genRandomInt(int rangeStart, int rangeEnd) {
    assert(rangeStart <= rangeEnd);
    int r;
    r = rangeStart + (int)((rangeEnd - rangeStart + 1.0) * random() / (RAND_MAX + 1.0));
    assert(r >= rangeStart && r <= rangeEnd);
    return r;
}

int HashFunctionFactory::genRandomJiShuInt(int rangeStart, int rangeEnd) {
    assert(rangeStart <= rangeEnd);
    rangeStart = rangeStart / 2;
    rangeEnd = rangeEnd / 2;
    int r;
    r = HashFunctionFactory::genRandomInt(rangeStart, rangeEnd);
    return 2 * r + 1;
}

void HashFunctionFactory::initRandom() {
    srandom(time(NULL));
}

BasicHashFunction::~BasicHashFunction() {
    free(this->a);
}