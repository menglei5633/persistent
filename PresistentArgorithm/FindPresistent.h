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
    int N;     //Number of items arrived in each epoch
    int T;     //Number of epochs
    int w;      //Number of item subsets
    int k;      //Number of BloomFilter's hash functions
    int L;      //Length of BloomFilter: B and b
    int c;      //Number of cells in each bucket
    int dimission;      //dimission of item
    int d;      //Number of candidate buckets for an item
    int m;      //Number of buckets in the SRAM
    int R;      //Memory space to stock an item
    int isReconstruction;       //weither to Reconstruction after T epoch
    int findNum;        //the number of item to find


};


class FindPresistent {
private:
    FindPresistentParams * fpp;
    BloomFilter* B;
    BloomFilter* b;
    HashTable* SRAM;
    HashTable** DRAM;
    HashFunction** d_hashFunctions;
    HashFunction* g;
    int t;      //current epoch index
    int n;      //current item index in current epoch

    int pre_processing(char* data);
    int recording(char* data);
    void post_processing();
//
//    int correct;
//    int wrong;



public:
    FindPresistent(FindPresistentParams* fpp);
//    void setFPP(FindPresistentParams * fpp);

    ~FindPresistent();
    void printRate();

    bool receive_a_item(char* data);
    int look_up_item(char* data_id, char* data_buffer);
    void list_reconstruction();

    void writePersistentToFile(FILE* file);

    void printHashTable();
};

#endif //SRC_FINDPRESISTENT_H
