//
// Created by lm on 18-9-4.
//

#ifndef SRC_FILEOPTION_H
#define SRC_FILEOPTION_H

#include <stdio.h>
#include <string>
#include <assert.h>
#include "../PresistentArgorithm/FindPresistent.h"


class DataFileOptions {
public:
    static void readAllDataToArray(FILE* fp, char** array, int lineNum);
    static void readOneLine(FILE* fp, char* buffer);
    static void readParam(std::string filename, FindPresistentParams* fpp);
    static void printParam(FindPresistentParams* fpp);
};



#endif //SRC_FILEOPTION_H
