//
// Created by lm on 18-9-2.
//
#define MAX_COUNT 1000

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "FindPresistent.h"


FindPresistent::FindPresistent(FindPresistentParams *fpp, FILE* file) {
    printf("start\n");
    this->fpp = fpp;
    this->B = new BloomFilter(fpp->dimission, fpp->k, fpp->L);
    this->b = new BloomFilter(*(this->B));
    this->SRAM = new HashTable(fpp->m, fpp->c, fpp->R);
    this->g = HashFunctionFactory::createBasicHashFunction(fpp->dimission, fpp->w);
    this->d_hashFunctions = (HashFunction **) malloc(fpp->d * sizeof(HashFunction *));

    assert(this->d_hashFunctions != NULL);
//    printf("aaaa\n");
    int i;
    for (i = 0; i < fpp->d; ++i) {
        this->d_hashFunctions[i] = HashFunctionFactory::createBasicHashFunction(fpp->dimission, fpp->m);
    }
//    printf("bbb\n");
//    printf("w:%d\n", this->fpp->w);
    this->DRAM = (HashTable **) malloc(fpp->w * sizeof(HashTable *));
//    printf("  %d \n", this->DRAM);
//    for (int i = 0; i < fpp->w; ++i) {
//        this->DRAM[i] = new HashTable(fpp->m, fpp->c, fpp->R);
//    }
//    assert(this->DRAM != NULL);
//    printf("ddddd\n");
//    for (i = 0; i < fpp->w; ++i) {

//        printf("fffff\n");
//        this->DRAM[i] = NULL;
//        printf("eeee\n");
//    }
//    printf("cccc\n");

    this->t = 0;
    this->n = 0;
    this->B->all_set_1();

//    this->B->printBitArray();

    this->b->all_set_0();

    this->file = file;

    this->tcam = new TCAM(fpp->TCAMSize, fpp->R);
    this->tcams = new TCAM*[fpp->w];

    //    this->b->printBitArray();
}
//
//void FindPresistent::setFPP(FindPresistentParams *fpp) {
//    this->fpp = fpp;
//}


int FindPresistent::pre_processing(char *data) {
//    printf("ggggg\n");
//    printf("pre_pocessing\n");
//    printf("====================\n");
//    this->B->printBitArray();
//    printf("\n");
//    this->b->printBitArray();
//    printf("====================\n");
    if (!this->B->is_inserted(data)) {
//        printf("is not in B\n");
        return 1;
    }

    if (this->b->is_inserted(data)) {
//        printf("is in b\n");
        return 2;
    }

//    printf("accept\n");

    this->b->add(data);
//    printf("T-t: %d    w: %d \n", this->fpp->T - this->t, this->fpp->w);
    if (this->fpp->T - this->t > this->fpp->w) {
//        printf("current epoch is not need to record\n");
        return 3;
    }
    unsigned int hashValue = this->g->hash(data);
//    printf("hashValue: %d  T-t : %d\n", hashValue, this->fpp->T - this->t - 1);
    if (hashValue == this->fpp->T - this->t - 1) {
//        printf("record\n");
        return 0;
    }
    return 4;
}

int FindPresistent::recording(char *data) {
//    printf("recording\n");
    unsigned int count = MAX_COUNT;
    unsigned int bucketIndex = this->fpp->d;
    int i;
    unsigned int hashValue;
    unsigned int hashCount;
    fprintf(this->file, "%s\n", data);
    if (this->fpp->m > 0) {
        for (i = 0; i < this->fpp->d; ++i) {
            hashValue = this->d_hashFunctions[i]->hash(data);
            hashCount = this->SRAM->countOfBucket[hashValue];
            if (hashCount < count) {
                count = hashCount;
                bucketIndex = hashValue;
            } else if (hashCount == count) {
                bucketIndex = bucketIndex > hashValue ? hashValue : bucketIndex;     // always go left
            }
        }
        if (this->SRAM->add(bucketIndex, data) == 0) {   //bucket has full
            return this->tcam->record(data);
        } else {
            return 1;
        }
    } else {
        return this->tcam->record(data);
    }

    //has get bucket index to insert


}

void FindPresistent::post_processing() {
//    printf("post_processing\n");
    int table = this->fpp->T - this->t;
//    printf("__***%d\n", this->t);
//    printf("table: %d\n", table);
    if (table <= this->fpp->w) {
        this->DRAM[table - 1] = new HashTable(*(this->SRAM));

//        printf("copy end\n");
//        printf("hashTable_____:\n");
//        this->SRAM->print();
//        printf("****************\n");
//        this->DRAM[table - 1]->print();
        this->SRAM->clear();

        this->tcams[table - 1] = new TCAM(*(this->tcam));
        this->tcam->clear();
//        for (int i = 0 ; i < this->tcams[table - 1]->getCount(); ++i) {
//            printf("after copy: %d,  %s\n", table - 1, this->tcams[table - 1]->read(i));
//        }

//        printf("clear end\n");
    }


//    char* bits_B = this->B->getBits();
    char* bits_b = this->b->getBits();
//    this->B->printBitArray();
//    this->b->printBitArray();
    this->B->weiYu(bits_b);

    this->b->all_set_0();
//    this->B->printBitArray();
//    this->b->printBitArray();

    if (this->t == this->fpp->T - 1) {

    }
}


bool FindPresistent::receive_a_item(char *data) {
//    if (this->t > this->fpp->T - 1 /*&& this->n == this->fpp->N*/) {
////        printf("**********************%d\n", this->t);
////        system("pause");
//        if (this->fpp->isReconstruction) {
//            this->list_reconstruction();
//        }
//        return false;
//    }
//    printf("t: %d\n", this->t);
//    printf("receive_data:%s\n", data);
//    this->B->printBitArray();
    int result = this->pre_processing(data);
    if (result == 0) {
//        printf("ffffffffffffffffff\n");
        this->recording(data);
    }

    this->n++;

    if (this->n == this->fpp->N) {
//        printf("epoch end\n");
        this->post_processing();
//        printf("vvvvv\n");
        this->n = 0;
        this->t ++;
//        printf("t: %d\n", this->t);
    }
    return true;
}

int FindPresistent::look_up_item(char *data_id, char *data_buffer) {
    if (!this->B->is_inserted(data_id)) {
        return 0;    //not in B
    }
    unsigned int hash_value;
    hash_value = this->g->hash(data_id);
    unsigned int table_index = hash_value;
    int i;
//    printf("aaaaa\n");
    for (i = 0; i < this->fpp->d; ++i) {
        hash_value = this->d_hashFunctions[i]->hash(data_id);
//        printf("bbbb %d\n", table_index);
        if (this->DRAM[table_index]->find(data_id, data_buffer, hash_value)) {
            return 1;     //find
        }
    }
    return 0;     //not find
}

void FindPresistent::list_reconstruction() {
    int i, j, k;
    HashTable* ht;
    char** bucket;
    for (i = 0; i < this->fpp->w; ++i) {
        for (j = 0; j < this->fpp->m; ++j) {
            bucket = this->DRAM[i]->returnBucket(j);
            int count = this->DRAM[i]->countOfBucket[j];
            k = 0;
            while (k < count) {
                if (this->B->is_inserted(bucket[k])) {
//                    printf("success\n");
                    ++k;
                } else {
//                    printf("drop____\n");
                    char* ptr = bucket[i];
                    bucket[i] = bucket[count-1];
                    free (ptr);
                    bucket[count - 1] = NULL;
                    --count;
                }
            }
            this->DRAM[i]->countOfBucket[j] = count;
        }
    }
}


void FindPresistent::printHashTable() {
    int i;
    for (i = 0; i < this->fpp->w; ++i) {
        this->DRAM[i]->print();
        printf("\n");
    }

}


FindPresistent::~FindPresistent() {
    free(this->B);
    free(this->b);
    free(this->SRAM);
    int i;
    for (i = 0; i < this->fpp->w; ++i) {
        free(this->DRAM[i]);
    }
    free(this->DRAM);
    free(this->g);
    for (i = 0; i < this->fpp->d; ++i) {
        free(this->d_hashFunctions[i]);
    }
    free(this->d_hashFunctions);
    for (i = 0; i < this->fpp->w; ++i) {
        free(this->tcams[i]);
    }
    free(this->tcams);
    free(this->tcam);
//    printf("findpersistent end\n");
}

//void FindPresistent::printRate() {
//    printf("correct: %d    wrong: %d  \n", this->correct, this->wrong);
//}

void FindPresistent::writePersistentToFile(FILE *file) {
    int i, j, k;
    HashTable * hashTable;
    char ** bucket;

    for (i = 0; i < this->fpp->w; ++i) {
        hashTable = this->DRAM[i];
        for (j = 0; j < this->fpp->m; ++j) {
            bucket = hashTable->buckets[j];
            for (k = 0; k < hashTable->countOfBucket[j]; ++k) {
                fprintf(file, "%s\n", this->DRAM[i]->buckets[j][k]);
            }
        }
    }
//    printf("bbv\n");
//    printf("remain:%d\n", this->TCAMRemainSize);{
    for (j = 0; j < this->fpp->w; ++j) {
//        printf("======j : %d  count : %d\n", j, this->tcams[j]->getCount());
        for (i = 0; i < this->tcams[j]->getCount(); ++i) {
//            printf("%s\n", this->tcams[j]->read(i));
            fprintf(file, "%s\n", this->tcams[j]->read(i));
        }
    }
}