//
// Created by lm on 18-9-3.
//
#include <stdio.h>
#include <stdlib.h>
#include "PresistentArgorithm/FindPresistent.h"
#include "BloomFilter/HashFunction.h"
#include "FileOption/FileOption.h"

int main(int argc, char* argv[]) {

    if (argc < 5) {
        return -1;
    }

    //read params
    char* dataFileName = argv[1];
    char* paramFileName = argv[2];
    char* writeFileName = argv[3];

//    int max_line = atoi(argv[4]);
    int start_line = atoi(argv[4]);

    //random init time
    HashFunctionFactory::initRandom();

//    int start_line = HashFunctionFactory::genRandomInt(0, max_line);

//    printf("start read line: %d\n", start_line);
    printf("algorthm start...\n");

    char input[100];
    FILE* file;




    //read algorithm params from file
    FindPresistentParams fpp;
    file = fopen(paramFileName, "r");
    DataFileOptions::readParam(file, &fpp);
    fclose(file);

    //print params
    DataFileOptions::printParam(&fpp);

//    int other_lookup = 5000;
    char lookup[100];

    FindPresistent fp = FindPresistent(&fpp);

    file = fopen(dataFileName, "r");

    //read data stream from file
    int i = 0;
    for (i = 0; i < start_line; ++i) {
        DataFileOptions::readOneLine(file, input);
    }
    for (i = 0; i < fpp.N * fpp.T; ++i) {
//        printf("%s\n", input);
        DataFileOptions::readOneLine(file, input);
        fp.receive_a_item(input);
//        printf("%d\n", i);
    }

    if (fpp.isReconstruction) {
        fp.list_reconstruction();
    }

//    printf("_______%d__________\n", i);

//    fp.printHashTable();

//    fp.list_reconstruction();

//    printf("\n\n");

//    fp.printHashTable();

//    fp.printRate();

    //find items from DRAM
    for (i = 0; i < fpp.findNum; ++i) {
        DataFileOptions::readOneLine(file, input);
        if (fp.look_up_item(input, lookup)) {
            printf ("%s  finded\n", lookup);
        }
    }

    fclose(file);

    //write persistent items finded to file
    file = fopen(writeFileName, "w");
    fp.writePersistentToFile(file);
    fclose(file);

    printf("algorithm end\n");
    return 0;
}
