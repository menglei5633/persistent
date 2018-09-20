//
// Created by lm on 18-9-2.
//

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
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

HashTable::HashTable(const HashTable &hashTable) {
    this->bucket_num = hashTable.bucket_num;
//    printf("bucket_num: %d\n", this->bucket_num);
    this->cell_num = hashTable.cell_num;
    this->entry_size = hashTable.entry_size;
//    printf("fafbabfja\n");
    this->buckets = (char***)malloc(this->bucket_num * sizeof(char**));
    int i, j;
//    printf("aaaaaa\n");
    for (i = 0; i < this->bucket_num; ++i) {
        this->buckets[i] = (char**)malloc(this->cell_num * sizeof(char*));
        for (j = 0; j < this->cell_num; ++j) {
            this->buckets[i][j] = (char*)malloc(this->entry_size * sizeof(char));
            memcpy(this->buckets[i][j], hashTable.buckets[i][j], this->entry_size * sizeof(char));
        }
    }
//    printf("ccccc\n");
    this->countOfBucket = (unsigned int *)malloc(this->bucket_num * sizeof(unsigned int));
    memcpy(this->countOfBucket, hashTable.countOfBucket, this->bucket_num * sizeof(unsigned int));
//    printf("fffffff\n");
}


void HashTable::clear() {
    int i;
    for (i = 0; i < this->bucket_num; ++i) {
        this->countOfBucket[i] = 0;
    }
}

void HashTable::print() {
    int i, j;
    printf("hashtable_________________________________\n");
    for (i = 0; i < this->bucket_num; ++i) {
        printf("bucket:%d\n", i);
        for (j = 0; j < this->countOfBucket[i]; ++j) {
            printf("%s\t",this->buckets[i][j]);
        }
        printf("\n\n");
    }
}


int HashTable::find(char *data_id, char *data_buffer, unsigned int bucket_index) {
    int i;
//    printf("ccccc\n");
//    printf("%d %d\n", bucket_index, this->cell_num);
    for (i = 0; i < this->countOfBucket[bucket_index]; ++i) {
//        printf("eeeee\n");
        if (strcmp(this->buckets[bucket_index][i], data_id) == 0) {
//            printf("dddd\n");
            memcpy(data_buffer, this->buckets[bucket_index][i], this->entry_size * sizeof(char));
            return 1;      //find
        }
    }
    return 0;    //not find
}

char** HashTable::returnBucket(unsigned int bucket_index) {
    return this->buckets[bucket_index];
}