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
//    printf("iiiiiii\n");
    int i;
    unsigned int value = 0;
//    printf("dimission: %d\n", this->dimission);
    for (i = 0; i < this->dimission; ++i) {
//        printf("%d %d\n", this->a[i], data[i]);
        value += this->a[i] * data[i];
    }
//    printf("value: %d\n", value);
    value = value % this->prime;
    return  value % this->b;
}

unsigned int BasicHashFunction::hash(char *data) {
//    printf("BasicHashStart\n");
//    unsigned int nLen = strlen((char*)data);
//    assert(nLen <= this->dimission);
//    unsigned char *p = (unsigned char*)data;
//    unsigned int value = 0;
//    unsigned int nIndex = 0;
//
//    while (*p) {
//        value += this->a[nIndex] * (*p);
//        ++p;
//        ++nIndex;
//    }
////    printf(" b: %d\n", this->b);
//    value = value % this->prime;
////    printf("%d ___\n", value);
//    return value % this->b;

    //another
    unsigned int value = 0;
    unsigned int nLen = strlen((char*)data);
    assert(nLen <= this->dimission);
    int i;
    for (i = 0; i < nLen; ++i) {
        //value += this->a[i] * pow(data[i], i);
        value += this->a[i] * data[i];
    }
    //printf("%d\n", value);
    value = value % this->prime;
    return value % this->b;
}

unsigned int BasicHashFunction::hash(int data) {
    unsigned int value = 0;
    int i;
    for (i = 0; i < this->dimission; ++i) {
        value += this->a[i] * pow(data, i);
    }
    value = value % this->prime;
    return value % this->b;
}


unsigned int BKDRHashFunction::hash(int *data) {
//    printf("nnnnnnnnnnn\n");
    unsigned int value = 0;
    int i;
    for (i = 0; i < this->dimission; ++i) {
        value += pow(this->seed, i) * data[i];
    }
    value = value % this->prime;
    return value % this->b;
}

unsigned int BKDRHashFunction::hash(char *data) {
//    printf("hashstart\n");
    unsigned int value = 0;
    unsigned int nLen = strlen((char*)data);
    assert(nLen < this->dimission);
//    printf("%s\n", data);
//    printf("seed: %d\n", this->seed);
    int i;
    for (i = 0; i < nLen; ++i) {
        value = value * this->seed + (unsigned int)data[i];
    }
//    printf("%d\n", value);
    value = value % this->prime;
//    printf("%d\n", value);
//    int j;
//    scanf("%d", &j);
    return value % this->b;
}

unsigned int BKDRHashFunction::hash(int data) {
    return 0;
}


HashFunction * HashFunctionFactory::createBasicHashFunction(unsigned int dimission, unsigned int b) {
//    assert(b != 0);
    int i;
    BasicHashFunction * bhf = new BasicHashFunction();
    bhf->a = (unsigned int*)malloc(dimission * sizeof(unsigned int));
    bhf->b = b;
    bhf->dimission = dimission;

    for (i = 0; i < dimission; ++i) {
        bhf->a[i] = (unsigned int)HashFunctionFactory::genRandomInt(0, 1000000);
//        printf("__%d\n", hf.a[i]);
    }
    return bhf;
}

HashFunction * HashFunctionFactory::createBKDRHashFunction(unsigned int dimission, unsigned int b) {
    assert(b != 0);
    int i;
    int seeds[]= {31, 131, 1313, 13331, 13131, 131313, 1313131, 13131313, 131313131, 1313131313};
    BKDRHashFunction * bhf = new BKDRHashFunction();
    bhf->dimission = dimission;
    bhf->b = b;

    bhf->seed = HashFunctionFactory::genRandomJiShuInt(1, 300);
//    printf("seed: %d\n", bhf->seed);

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

HashFunction* HashFunctionFactory::copyBKDRHashFunction(BKDRHashFunction *hashFunction) {
    BKDRHashFunction* bhf = new BKDRHashFunction();
    bhf->dimission = hashFunction->dimission;
    bhf->seed = hashFunction->seed;
    bhf->b = hashFunction->b;
    return bhf;
}

HashFunction* HashFunctionFactory::copyBasicHashFunction(BasicHashFunction *hashFunction) {
    BasicHashFunction* bhf = new BasicHashFunction();
    bhf->dimission = hashFunction->dimission;
    bhf->b = hashFunction->b;
    bhf->a = (unsigned int*)malloc(bhf->dimission * sizeof(unsigned int));
    memcpy(bhf->a, hashFunction->a, bhf->dimission * sizeof(unsigned int));
//    printf("_______________\n");
//    int i;
//    printf("________d:%d\n", bhf->dimission);
//    for (i = 0; i < bhf->dimission; ++i) {
//        printf("%d %d\n", hashFunction->a[i], bhf->a[i]);
//    }

    return bhf;
}

BasicHashFunction::~BasicHashFunction() {
//    printf("hhhh\n");
    free(this->a);
}
