//
// Created by lm on 18-9-1.
//

#ifndef SRC_FINDPRESISTENT_H
#define SRC_FINDPRESISTENT_H



#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../BloomFilter/BloomFilter.h"
#include "../HashTable/HashTable.h"
//#include "../BloomFilter/HashFunction.h"


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
    int TCAMSize;   //the size of tcam

};


class TCAM {
public:
    TCAM(int size, int R) {
        this->size = size;
        this->R = R;
        this->count = 0;
        if (size > 0) {
            this->datas = new char*[size];
            for (int i = 0; i < size; ++i) {
                this->datas[i] = new char[R];
            }
        } else {
            this->datas = nullptr;
        }
    }
    TCAM() {
        this->size = 0;
        this->count = 0;
        this->datas = nullptr;
        this->R = 0;
    }

    TCAM(const TCAM &tcam) {
        this->size = tcam.size;
        this->count = tcam.count;
        this->R = tcam.R;
        if (this->size > 0) {
            this->datas = new char*[size];
            for (int i = 0; i < size; ++i) {
                this->datas[i] = new char[R];
            }
            for (int i = 0; i < count; ++i) {
                memcpy(this->datas[i], tcam.datas[i], this->R);
//                printf("copy : %s\n", this->datas[i]);
            }
        } else {
            this->datas = nullptr;
        }
    }

    int record(char* data) {
        if (this->count >= this->size) {
            return 0;
        } else {
            memcpy(this->datas[this->count], data, this->R);
            this->count++;
            return 1;
        }
    }

    char* read(int location) {
        if (location < count) {
            return this->datas[location];
        } else {
            return nullptr;
        }
    }

    int getCount() {
        return this->count;
    }

    void clear() {
        this->count = 0;
    }

    ~TCAM() {
        if (this->size <= 0) {
            return;
        }
        for (int i = 0; i < this->size; ++i) {
            free(this->datas[i]);
        }
        free(this->datas);
    }
private:
    int size;
    int count;
    int R;
    char** datas;
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
    FILE* file;
    int t;      //current epoch index
    int n;      //current item index in current epoch

    TCAM* tcam;
    TCAM** tcams;

    int pre_processing(char* data);
    int recording(char* data);
    void post_processing();
//
//    int correct;
//    int wrong;



public:
    FindPresistent(FindPresistentParams* fpp, FILE* file);
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
