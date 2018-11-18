//
// Created by lm on 18-9-27.
//
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc < 6) {
        return -1;
    }
    int c = atoi(argv[1]);
    int entrySize = atoi(argv[2]);
    int L = atoi(argv[3]);
    int m = atoi(argv[4]);
    int tcamsize = atoi(argv[5]);

    float result = 1.0* (L + m * entrySize * c * 8 + tcamsize * entrySize * 8) / 8;

    printf("%f", result);

    return 0;

}
