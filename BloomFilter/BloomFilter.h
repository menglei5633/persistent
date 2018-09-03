//
// Created by lm on 18-9-1.
//

#ifndef SRC_BLOOMFILTER_H
#define SRC_BLOOMFILTER_H

#include "HashFunction.h"
#include "BitArray.h"

class BloomFilter {
private:
    BitArray* bitArray;
    HashFunction** hashFunctions;
    unsigned int dimission;
    unsigned int k;
    unsigned int bitSize;
public:
    BloomFilter(int dimission, int k, int bitSize);
    void add(int * data);
    void add(char * data);
    bool is_inserted(int * data);
    bool is_inserted(char * data);
    void all_set_1();
    void all_set_0();
    ~BloomFilter();
};

#endif //SRC_BLOOMFILTER_H
