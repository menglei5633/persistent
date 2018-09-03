//
// Created by lm on 18-9-2.
//

#include <assert.h>
#include <string.h>
#include "HashTable.h"

HashTable::HashTable(unsigned int m, unsigned int c, unsigned int R) {
    this->bucket_num = m;
    this->cell_num = c;
    this->entry_size = R;
    this->buckets = (char***)malloc(m * sizeof(char**));
    assert(this->buckets != NULL);
    int i, j;
    for (i = 0; i < m; ++i) {
        this->buckets[i] = (char**)malloc(c * sizeof(char*));
        assert(this->buckets[i] != NULL);
        for (j = 0; j < c; ++j) {
            this->buckets[i][j] = (char*)malloc(R * sizeof(char));
            assert(this->buckets[i][j] != NULL);
        }
    }

    this->countOfBucket = (unsigned int*)malloc(m * sizeof(unsigned int));
    assert(this->countOfBucket != NULL);

}

int HashTable::add(unsigned int bucketIndex, char* data) {
    if (this->countOfBucket[bucketIndex] < this->cell_num) {
        char * to_insert = this->buckets[bucketIndex][this->countOfBucket[bucketIndex]];
        memcpy(to_insert, data, strlen(data) + 1);
        this->countOfBucket[bucketIndex]++;
        return 1;
    } else {
        return 0;
    }

}

