#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <assert.h>
//#include "RandomStruct.h"
#include "sha1.hpp"

using namespace std;

int genRandomInt(int rangeStart, int rangeEnd) {
    assert(rangeStart <= rangeEnd);
    int r;
    r = rangeStart + (int)((rangeEnd - rangeStart + 1.0) * random()/ (RAND_MAX + 1.0));
    assert(r >= rangeStart && r <= rangeEnd);
    return r;
}

int main(int argc, char* argv[]) {

    if (argc < 10) {
        return -1;
    }
    int all_num, per_num, s, T, n, N, F;
    char * fileName, *writeFileName;

    all_num = atoi(argv[1]);
    per_num = atoi(argv[2]);
    s = atoi(argv[3]);
    T = atoi(argv[4]);
    N = atoi(argv[5]);
    n = atoi(argv[6]);
    F = atoi(argv[7]);

    fileName = argv[8];    //stream file
    writeFileName = argv[9];   //persistent file
    ofstream openfile(fileName, ios::out);

    int step = 1;

    SHA1 sha1;

    srandom(time(NULL));
    int i, j, k;
    int* epoch_item = (int*)malloc(n * sizeof(int));
    int* other_item = (int*)malloc((N - per_num) * sizeof(int));
    int* card = (int*)malloc((all_num - per_num) * sizeof(int));
    int* card1 = (int*)malloc((n) * sizeof(int));

    for (i = 0; i < all_num - per_num; ++i) {
        card[i] = s;
    }



//    RandomGenerate randomGenerate1 = RandomGenerate(per + 1 , all_num);
    for (i = 0; i < T; ++i) {
        int start = genRandomInt(per_num, all_num - 1);
        for (j = 0; j < n; ++j) {
            card1[j] = 0;
        }
        printf("T: %d\n", i);
        for (j = 0; j < N - per_num; ++j) {
//            printf("bbbb\n");
//            printf("start::: %d\n", start);
            while (card[start - per_num] <= 0) {
                start = start + step;
                if (start >= all_num) {
                    start = start - all_num + per_num;
                }
            }
            other_item[j] = start;
            --card[start - per_num];

//            string input = to_string(start);
////            printf("%s\n", input);
////            cout<< input <<endl;
//
//            sha1.update(input);
////            printf("%s\n", sha1.final());
//            openfile << sha1.final() << endl;
//            printf("aaaa\n");
//            printf("start:: %d\n", start);

//            printf("ccc\n");
            start = start + step;
            if (start >= all_num) {
                start = start - all_num + per_num;
            }
        }

//        printf("kkkk\n");

        for (j = 0; j < N; ++j) {
//            string input = to_string(j);
//            sha1.update(input);
//            openfile << sha1.final() << endl;
            int loc = genRandomInt(0, n - 1);
            while (card1[loc] == 1) {
                loc = (loc + 1) % n;
//                printf("card1 : %d\n", card1[loc]);
            }
//            printf("_________________\n");
            if (j < per_num) {
                epoch_item[loc] = j;
            } else {
                epoch_item[loc] = other_item[j - per_num];
            }
//            printf("aaaaaa\n");
            card1[loc] = 1;
        }
        for (j = 0; j < n; ++j) {
            if (card1[j] == 0) {
                int loc = genRandomInt(0, N - 1);
                while (loc >= per_num && card[other_item[loc - per_num] - per_num] <= 0) {
                    loc = (loc + 1) % N;
                }
                if (loc < per_num) {
                    epoch_item[j] = loc;
                } else {
                    epoch_item[j] = other_item[loc - per_num];
                    --card[epoch_item[j] - per_num];
                }
                card1[j] = 1;
            }
        }

        for (j = 0; j < n; ++j) {
            string input = to_string(epoch_item[j]);
            sha1.update(input);
            openfile << sha1.final() << endl;
        }

    }

    for (i = 0; i < F; ++i) {
        string input = to_string(genRandomInt(0, all_num-1));
        sha1.update(input);
        openfile << sha1.final() << endl;
    }
    printf("ddddd\n");
    openfile.close();

    ofstream openfile1(writeFileName, ios::out);
    for (i = 0; i < per_num; ++i) {
        string input = to_string(i);
        sha1.update(input);
        openfile1 << sha1.final() << endl;
    }

    openfile1.close();
//    free(other_item);
//    free(persitent_item);

    return 0;

}
