//
// Created by lm on 18-9-1.
//

#ifndef SRC_FINDPRESISTENT_H
#define SRC_FINDPRESISTENT_H

#include "../BloomFilter/BloomFilter.h"
#include "../HashTable/HashTable.h"
#include "../BloomFilter/HashFunction.h"


//used for algorithm params
class FindPresistentParams {
public:
    int N;     //Number of distinct items arrived in each epoch
    int T;     //Number of epochs
    int w;      //Number of item subsets
    int k;      //Number of BloomFilter's hash functions
    int L;      //Length of BloomFilter: B and b
    int c;      //Number of cells in each bucket
    int dimission;      //dimission of item
    int d;      //Number of candidate buckets for an item
    int m;      //Number of buckets in the SRAM
    int R;      //Memory space to stock an item
    int t;      //current epoch index


};


class FindPresistent {
private:
    FindPresistentParams * fpp;
    BloomFilter* B;
    BloomFilter* b;
    HashTable* hashTable;
    HashFunction** d_hashFunctions;
    HashFunction* g;

public:
    FindPresistent(FindPresistentParams* fpp);
//    void setFPP(FindPresistentParams * fpp);
    bool pre_processing(char* data);
    int recording(char* data);
};

#endif //SRC_FINDPRESISTENT_H
