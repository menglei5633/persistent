//
// Created by lm on 18-9-2.
//
#define MAX_COUNT 1000

#include "FindPresistent.h"


FindPresistent::FindPresistent(FindPresistentParams *fpp) {
    this->fpp = fpp;
    this->B = new BloomFilter(fpp->dimission, fpp->k, fpp->L);
    this->b = new BloomFilter(fpp->dimission, fpp->k, fpp->L);
    this->hashTable = new HashTable(fpp->m, fpp->c, fpp->R);
    this->g = HashFunctionFactory::createBKDRHashFunction(fpp->dimission, fpp->w);
    this->d_hashFunctions = (HashFunction**)malloc(fpp->d * sizeof(HashFunction*));

    int i;
    for (i = 0; i < fpp->d; ++i) {
        this->d_hashFunctions[i] = HashFunctionFactory::createBKDRHashFunction(fpp->dimission, fpp->d);
    }

    this->fpp->t = 0;
    this->B->all_set_1();
    this->b->all_set_0();
}
//
//void FindPresistent::setFPP(FindPresistentParams *fpp) {
//    this->fpp = fpp;
//}


bool FindPresistent::pre_processing(char *data) {
    if (!this->B->is_inserted(data)) {
        return false;
    }
    if (this->b->is_inserted(data)) {
        return false;
    }
    this->b->add(data);
    unsigned int hashValue = this->g->hash(data);
    if (hashValue == T - t) {

    }
}

int FindPresistent::recording(char *data) {
    unsigned int count = MAX_COUNT;
    unsigned int bucketIndex = this->fpp->d;
    int i;
    unsigned int hashValue;
    unsigned int hashCount;
    for (i = 0; i < this->fpp->d; ++i) {
        hashValue = this->d_hashFunctions[i]->hash(data);
        hashCount = this->hashTable->countOfBucket[hashValue];
        if (hashCount < count) {
            count = hashCount;
            bucketIndex = hashValue;
        } else if (hashCount == count) {
            bucketIndex = bucketIndex > hashValue ? hashValue : bucketIndex;     // always go left
        }
    }

    //has get bucket index to insert
    this->hashTable->add(bucketIndex, data);

}