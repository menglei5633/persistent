//
// Created by lm on 18-9-2.
//

#ifndef SRC_HASHTABLE_H
#define SRC_HASHTABLE_H


class HashTable {
public:
    HashTable(unsigned int m, unsigned int c, unsigned int R);
    unsigned int* countOfBucket;
    int add(unsigned int bucketIndex, char* data);
private:
    char *** buckets;
    unsigned int bucket_num;
    unsigned int cell_num;
    unsigned int entry_size;      //字节数
};

#endif //SRC_HASHTABLE_H
