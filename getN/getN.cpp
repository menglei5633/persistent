//
// Created by lm on 18-9-11.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc < 5) {
        return -1;
    }

    int N = atoi(argv[1]);
    int T = atoi(argv[2]);
    int start_line = atoi(argv[3]);
//    int line_num = atoi(argv[4]);
    char* read_file = argv[4];

    int max_n = 0, count;

    int i, j, k;

    char** epoch_items = (char**)malloc(N * sizeof(char*));
    for (i = 0;  i < N; ++i) {
        epoch_items[i] = (char*)malloc(100 * sizeof(char));
    }

    FILE* fp = fopen(read_file, "r");

    char buffer[100];
    for (i = 0; i < start_line; ++i) {
        fscanf(fp, "%s", buffer);
    }

    printf("start\n");

    for (i = 0; i < T; ++i) {
        count = 0;
        printf("T : %d\n", i);
        for (k = 0; k < N; ++k) {
//            printf("k : %d\n", k);
            fscanf(fp, "%s", buffer);
            for (j = 0; j < count; ++j) {
                if (strcmp(buffer, epoch_items[j]) == 0) {
                    break;
                }
            }
            if (j == count) {
                memcpy(epoch_items[count], buffer, 100);
                ++count;
            }
        }
        if (count > max_n) {
            max_n = count;
        }
    }

    fclose(fp);

    printf("max_N : %d\n", max_n);

    for (i = 0; i < N; ++i) {
        free(epoch_items[i]);
    }
    free(epoch_items);

    return 0;

}
