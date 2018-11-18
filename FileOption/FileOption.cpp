//
// Created by lm on 18-9-4.
//
#include "FileOption.h"

void DataFileOptions::readOneLine(FILE* fp, char* buffer) {
    fscanf(fp, "%s", buffer);
}

void DataFileOptions::readAllDataToArray(FILE *fp, char **array, int lineNum) {
    int i;
    for (i = 0; i < lineNum; ++i) {
        fscanf(fp, "%s", array[i]);
    }
}

void DataFileOptions::readParam(FILE *fp, FindPresistentParams *fpp) {
    char buffer[50];

    fscanf(fp, "%s", buffer);
    fscanf(fp, "%d", &(fpp->N));

    fscanf(fp, "%s", buffer);
    fscanf(fp, "%d", &(fpp->T));

    fscanf(fp, "%s", buffer);
    fscanf(fp, "%d", &(fpp->c));

    fscanf(fp, "%s", buffer);
    fscanf(fp, "%d", &(fpp->R));

    fscanf(fp, "%s", buffer);
    fscanf(fp, "%d", &(fpp->w));


    fscanf(fp, "%s", buffer);
    fscanf(fp, "%d", &(fpp->k));

    fscanf(fp, "%s", buffer);
    fscanf(fp, "%d", &(fpp->L));

    fscanf(fp, "%s", buffer);
    fscanf(fp, "%d", &(fpp->dimission));

    fscanf(fp, "%s", buffer);
    fscanf(fp, "%d", &(fpp->d));

    fscanf(fp, "%s", buffer);
    fscanf(fp, "%d", &(fpp->m));


    fscanf(fp, "%s", buffer);
    fscanf(fp, "%d", &(fpp->isReconstruction));

    fscanf(fp, "%s", buffer);
    fscanf(fp, "%d", &(fpp->findNum));

    fscanf(fp, "%s", buffer);
    fscanf(fp, "%d", &(fpp->TCAMSize));


}

void DataFileOptions::printParam(FindPresistentParams *fpp) {
    printf("N : %d\n", fpp->N);
    printf("T : %d\n", fpp->T);
    printf("w : %d\n", fpp->w);
    printf("k : %d\n", fpp->k);
    printf("L : %d\n", fpp->L);
    printf("c : %d\n", fpp->c);
    printf("dimission : %d\n", fpp->dimission);
    printf("d : %d\n", fpp->d);
    printf("m : %d\n", fpp->m);
    printf("R : %d\n", fpp->R);
    printf("isReconstruction : %d\n", fpp->isReconstruction);
    printf("tcamsize: %d\n", fpp->TCAMSize);

    printf("fndnum : %d\n", fpp->findNum);

}
