//
// Created by lm on 18-9-11.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc < 6) {
        return -1;
    }

    int N = atoi(argv[1]);
    int T = atoi(argv[2]);
    int start_line = atoi(argv[3]);
    char* read_file = argv[4];
    char* write_file = argv[5];
//    int w = atoi(argv[6]);

    printf("exact find start\n");
    printf("N : %d\n", N);
    printf("T : %d\n", T);
    printf("start line : %d\n", start_line);
    printf("read file : %s\n", read_file);
    printf("write file : %s\n", write_file);


    int i, j, k;

    char** persistent_items = (char**)malloc(N * sizeof(char*));
    for (i = 0;  i < N; ++i) {
        persistent_items[i] = (char*)malloc(100 * sizeof(char));
    }
    int* card = (int*)malloc(N * sizeof(int));

    FILE* fp = fopen(read_file, "r");

    char buffer[100];
    for (i = 0; i < start_line; ++i) {
        fscanf(fp, "%s", buffer);
    }

    printf("start\n");

    for (i = 0; i < N; ++i) {
        fscanf(fp, "%s", persistent_items[i]);
        card[i] = 0;
    }

    for (i = 1; i <= T - 1; ++i) {
        printf("T : %d\n", i);
        for (k = 0; k < N; ++k) {
            fscanf(fp, "%s", buffer);
            for (j = 0; j < N; ++j) {
                if (card[j] == 0 && strcmp(buffer, persistent_items[j]) == 0) {
//                    printf("get\n");
                    card[j] = 1;
                }
            }
        }
        for (j = 0; j < N; ++j) {
            if (card[j] == 0) {
                card[j] = -1;
            } else if (card[j] == 1) {
                card[j] = 0;
            }
        }
    }

    fclose(fp);

    for (i = 1; i < N; ++i) {
        if (card[i] == -1) {
            continue;
        }
        for (j = 0; j < i; ++j) {
            if (card[j] == 0 && strcmp(persistent_items[i], persistent_items[j]) == 0) {
                card[i] = -1;
                break;
            }
        }
    }

    fp = fopen(write_file, "w");

    for (i = 0; i < N; ++i) {
        if (card[i] != -1) {
            fprintf(fp, "%s\n", persistent_items[i]);
        }
    }
    fclose(fp);

    for (i = 0; i < N; ++i) {
        free(persistent_items[i]);
    }
    free(persistent_items);
    free(card);

    printf("exact find end\n");

    return 0;

}
