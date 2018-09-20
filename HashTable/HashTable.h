//
// Created by lm on 18-9-2.
//

#ifndef SRC_HASHTABLE_H
#define SRC_HASHTABLE_H


class HashTable {
public:
    HashTable(unsigned int m, unsigned int c, unsigned int R);
    HashTable(const HashTable& hashTable);
    unsigned int* countOfBucket;
    int add(unsigned int bucketIndex, char* data);
    int find(char* data_id, char* data_buffer, unsigned int bucket_index);
    char** returnBucket(unsigned int bucket_index);
    void clear();
    void print();
    char *** buckets;
private:

    unsigned int bucket_num;
    unsigned int cell_num;
    unsigned int entry_size;      //字节数
};

#endif //SRC_HASHTABLE_H
