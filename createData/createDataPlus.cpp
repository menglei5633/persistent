#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <assert.h>
//#include "RandomStruct.h"
//#include "sha1.hpp"

using namespace std;

int genRandomInt(unsigned int rangeStart, unsigned int rangeEnd) {
    assert(rangeStart <= rangeEnd);
    int r;
    r = rangeStart + (int)((rangeEnd - rangeStart + 1.0) * random()/ (RAND_MAX + 1.0));
    assert(r >= rangeStart && r <= rangeEnd);
    return r;
}



void intToChar(int num, char* str) {
    int i, j;
//    assert(num >=0);

    for (i = 0; i < 4; ++i) {
        j = (int)(num % 94);
//        printf("num : %d\n", num);
//        printf("95 pow %d : %f\n", i + 1, 95);
//        printf("j : %d\n", j);
        assert(j >= 0);
        str[3 - i] = (char)(j + 33);
        assert(str[3 - i] >= 33);
        num = num / 94;
    }
}

void writeToFile(FILE* file, char* str) {
    for (int i = 0; i < 4; ++i) {
        fprintf(file, "%c", str[i]);
    }

//    fprintf(file, "%s\n", str);
    fprintf(file, "\n");
}

int main(int argc, char* argv[]) {
    if (argc < 9) {
        return -1;
    }
    int all_num, per_num, S, T, n, N, F, a;
    char * all_file, *per_file;

    int allItemNum;

    all_num = atoi(argv[1]);
    per_num = atoi(argv[2]);
    S = atoi(argv[3]);
    T = atoi(argv[4]);
//    N = atoi(argv[5]);
//    n = atoi(argv[6]);
    F = atoi(argv[5]);

    a = atoi(argv[6]);

    all_file = argv[7];    //stream file
    per_file = argv[8];   //persistent file
//    ofstream openfile(fileName, ios::out);

    n = all_num / T;

    unsigned int max_value = 1 << 31;
//    printf("%u", max_value);


    FILE * file = fopen(all_file, "w");

    int step = 1;
    char str[4];

//    SHA1 sha1;

    srandom(time(NULL));
    int i, j, k;
    unsigned int* all_item = (unsigned int*)malloc(all_num * sizeof(unsigned int));
    unsigned int* epoch_item = (unsigned int*)malloc(n * sizeof(unsigned int));
    unsigned int* spread = (unsigned int*)malloc(all_num * sizeof(unsigned int));
    unsigned int* spread_backup = (unsigned int*)malloc(all_num * sizeof(unsigned int));
    int* card1 = (int*)malloc((n) * sizeof(int));
    int* card = (int*)malloc((all_num) * sizeof(int));

    for (i = 0; i < all_num; ++i) {
        card[i] = 0;
    }
    for (i = 0; i < all_num; ++i) {
        unsigned int item = genRandomInt(1, all_num);
//        for (j = 0; j < i; ++j) {
//            if (item == all_item[j]) {
//                break;
//            }
//        }
//        if (j == i) {
//            all_item[i] = item;
//        }
        while (card[item] != 0) {
            item = (item + 1);
            if (item > all_num) {
                item = 1;
            }
        }
        all_item[i] = item;
        card[item] = 1;
//        printf("%u\n", item);
    }

    printf("create data ok\n");
    FILE * ff = fopen("ddd.txt", "w");
    for (i = 0; i < all_num; ++i) {
        fprintf(ff, "%u\n", all_item[i]);
    }
    fclose(ff);


    // 设置spread
    for (i = 0; i < per_num; ++i) {
        spread[i] = T;
        spread_backup[i] = T;
    }
    for (i = per_num; i < all_num; ++i) {
        spread[i] = 0;
        spread_backup[i] = 0;
    }

    int M = 0;
    double SS = 0;
    for (i = 1; i <= S; ++i) {
        SS += pow(1.0 / i, a);
//        printf("pow : %f\n", pow(1 / i, a));
//        printf("SS : %f\n", SS);
    }
//    printf("SS: %f\n", SS);
    for (i = 1; i <= S; ++i) {
        double fs = 1.0 / (pow(i, a) * SS);
        int num = fs * all_num / 2;
//        printf("fs:%d\n", (int)(fs * all_num));
        M += num * i;
        for (j = 0; j < num; ++j) {
            int loc = genRandomInt(per_num, all_num - 1);
            while (spread[loc] != 0) {
                loc = (loc + 1);
                if (loc >= all_num) {
                    loc = per_num;
                }
            }
            spread[loc] = i;
            spread_backup[loc] = i;
        }

    }

    printf("M : %d", M);
    int no_per_epoch = n / 2;

    printf("%d\n", no_per_epoch);

    unsigned int* epoch_item1 = (unsigned int*)malloc((per_num + no_per_epoch) * sizeof(unsigned int));

    //生成epoch
    for (i = 0; i < T; ++i) {
        printf("T : %d\n", i);
        j = 0;
        k = 0;
        while (k < per_num + no_per_epoch) {
            if (spread[j] > 0) {
                epoch_item1[k] = all_item[j];
                --spread[j];
//                printf("%u\n", epoch_item1[k]);
                ++k;
            }
            j = (j + 1);
            if (j >= all_num) {
                j = 0;
                printf("again!!!!!!!\n");
                int g;
                scanf("%d", &g);
            }
        }
//        printf("ooooo\n");
//        int g;
//        scanf("%d", &g);

        for (j = 0; j < n; ++j) {
            card1[j] = 0;
        }

        for (j = 0; j < per_num + no_per_epoch; ++j) {
            int loc = genRandomInt(0, n - 1);
            while (card1[loc] != 0) {
                loc = (loc + 1) % n;
            }
            epoch_item[loc] = epoch_item1[j];
            card1[loc] = 1;
        }
        for (j = 0; j < n; ++j) {
            if (card1[j] == 0) {
                int loc = genRandomInt(0, per_num + no_per_epoch - 1);
                epoch_item[j] = epoch_item1[loc];
            }
        }

        //write to file
        for (j = 0; j < n; ++j) {
            intToChar((int)epoch_item[j], str);
            writeToFile(file, str);
        }
    }
    for (i = 0; i < F; ++i) {
        int find = genRandomInt(0, all_num - 1);
        intToChar((int)all_item[find], str);
        writeToFile(file, str);
    }
    printf("write all file end\n");
    fclose(file);

    file = fopen(per_file, "w");

    for (i = 0; i < per_num; ++i) {
        intToChar((int)all_item[i], str);
        writeToFile(file, str);
    }
    fclose(file);
    printf("write per file end\n");

    allItemNum = 0;
    for (i = 0; i < all_num; ++i) {
        if (spread_backup[i] > spread[i]) {
            allItemNum++;
        }
    }
    printf("allItemNum : %d\n", allItemNum);

    free(all_item);
    printf("aaaa\n");
    free(epoch_item);
    printf("bbb\n");

    free(epoch_item1);
    printf("ccc\n");

    free(spread);
    free(spread_backup);
    printf("ddd\n");

    free(card1);

    return 0;

}
