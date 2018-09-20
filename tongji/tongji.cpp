//
// Created by lm on 18-9-8.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


int main(int argc, char* argv[]) {
    if (argc < 5) {
        return -1;
    }
//    printf("%s\n", argv[1]);
    int per_num = atoi(argv[1]);
    int res_num = atoi(argv[2]);
    char* perFileName = argv[3];
    char* resFileName = argv[4];
//    printf("p: %d  r: %d\n", per_num, res_num);
    printf("tongji start\n");

    int correct = 0;
    int wrong = 0;
    int i, j;

    char ** perArray = (char**)malloc(per_num * sizeof(char*));
    for (i = 0; i < per_num; ++i) {
        perArray[i] = (char*)malloc(50 * sizeof(char));
    }

    FILE * fp = fopen(perFileName, "r");


    for (i = 0; i < per_num; ++i) {
        fscanf(fp, "%s", perArray[i]);
    }
//    printf("read end\n");
    fclose(fp);

    fp = fopen(resFileName, "r");
    assert(fp != NULL);
    char buffer[50];
    for (i = 0; i < res_num; ++i) {
        fscanf(fp, "%s", buffer);
//        if (strcmp(buffer, "") == 0) {
//            continue;
//        }
//        printf("a\n");
        for (j = 0; j < per_num; ++j) {
            if (strcmp(buffer, perArray[j]) == 0) {
                correct += 1;
                break;
            }
        }
        if (j == per_num) {
            wrong += 1;
        }
    }

    printf("all: %d    correct: %d    wrong: %d\n", per_num, correct, wrong);

    printf("tongji end\n");

    return 0;
}
