//
// Created by lm on 18-9-25.
//
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>


class FindPresistentParams {
public:
    int N;     //Number of items arrived in each epoch
    int T;     //Number of epochs
    int w;      //Number of item subsets
    //int k;      //Number of BloomFilter's hash functions
    int L;      //Length of BloomFilter: B and b
    int c;      //Number of cells in each bucket
    //int dimission;      //dimission of item
    int d;      //Number of candidate buckets for an item
    int m;      //Number of buckets in the SRAM
    int R;      //Memory space to stock an item
    //int isReconstruction;       //weither to Reconstruction after T epoch
    //int findNum;        //the number of item to find
    int tcamsize;   //the size of tcam
    char *paramFile;
    char *daraFile;
    char *resFile;
    char *resFile2;
    int startline;
    char *exactFileName;
    float target_fn;
    float target_fp;
    int numOfAgain;
    int endL;
    int allItemNum;
    float mem;
};

void computeMem(FindPresistentParams &fpp) {
    char cmd[200];
    sprintf(cmd, "./computeMemory/a.out %d %d %d %d %d", fpp.c, fpp.R, fpp.L, fpp.m, fpp.tcamsize);
    //printf("%s\n", cmd);
    FILE *cmdfp = popen(cmd, "r");
    assert(cmdfp != NULL);
    fscanf(cmdfp, "%f", &fpp.mem);
    //printf("mem: %f\n", fpp.mem);
}

void printParam(FindPresistentParams &fpp) {
    printf("w: %d, tcam: %d, c: %d, d: %d, N: %d, T: %d, startline: %d, datafile: %s\n", fpp.w, fpp.tcamsize, fpp.c,
           fpp.d, fpp.N, fpp.T, fpp.startline, fpp.daraFile);
}

void printCmd(FindPresistentParams fpp, char *cmd) {
    sprintf(cmd, "./algorithm.sh %s %s %s %s %d %s %d %d %d %d %d %d %d %d", fpp.paramFile, fpp.daraFile, fpp.resFile,
            fpp.resFile2, fpp.startline, fpp.exactFileName, fpp.N, fpp.T, fpp.L, fpp.m, fpp.tcamsize, fpp.w, fpp.c,
            fpp.d);
//    printf("%s\n", cmd);
}

void do_cmd(FindPresistentParams &fpp, int &all_allper, int &all_correct, int &all_wrong) {
    char cmd[200];
    int all_per, correct, wrong;
    printCmd(fpp, cmd);
    FILE *cmdfp = popen(cmd, "r");
    assert(cmdfp != NULL);
    fscanf(cmdfp, "%d %d %d", &all_per, &correct, &wrong);
    pclose(cmdfp);
//    printf("%d %d %d\n", all_per, correct, wrong);
    all_allper += all_per;
    all_correct += correct;
    all_wrong += wrong;
}

void test_one_time(FindPresistentParams &fpp) {
    //printParam(fpp);
    int all_allper = 0, all_correct = 0, all_wrong = 0;
    float min_mem = 1000000000;
    int min_L = 1000000000;
    int min_m = 1000000000;
    float mem;

    float Pfp = 1;
    float Pfn = 1;
    int i;

    for (i = 0; i < fpp.numOfAgain; ++i) {

        do_cmd(fpp, all_allper, all_correct, all_wrong);
    }
    Pfn = 1.0 * (all_allper - all_correct) / all_allper;
   // printf("pfn: %f\n", Pfn);

    printf("all_allper: %d, all_correct: %d, allwrong: %d\n", all_allper, all_correct, all_wrong);
    if (Pfn <= fpp.target_fn) {
        while (Pfn <= fpp.target_fn && fpp.m > 0) {
            all_allper = all_correct = all_wrong = 0;
            --fpp.m;
            printf("trace m for Pfn: %d\n", fpp.m);
            for (i = 0; i < fpp.numOfAgain; ++i) {
                do_cmd(fpp, all_allper, all_correct, all_wrong);
            }
//            printf("angin num : %d   all_allper: %d     all_correct: %d     all_wrong: %d\n",
//                   numOfAgain, all_allper, all_correct, all_wrong);
            Pfn = 1.0 * (all_allper - all_correct) / all_allper;
    //        printf("trace Pfn: %f\n", Pfn);
        }
        ++fpp.m;
    } else {
        while (Pfn > fpp.target_fn) {
            all_allper = all_correct = all_wrong = 0;
            ++fpp.m;
            printf("trace m for Pfn: %d\n", fpp.m);
            for (i = 0; i < fpp.numOfAgain; ++i) {
                do_cmd(fpp, all_allper, all_correct, all_wrong);
            }
//            printf("angin num : %d   all_allper: %d     all_correct: %d     all_wrong: %d\n",
//                   numOfAgain, all_allper, all_correct, all_wrong);
            Pfn = 1.0 * (all_allper - all_correct) / all_allper;
           // printf("trace Pfn: %f\n", Pfn);
        }
    }
    while (Pfn <= fpp.target_fn && fpp.L > fpp.endL) {
        all_allper = all_correct = all_wrong = 0;
        fpp.L -= 50;
        printf("trace L for Pfn: %d\n", fpp.L);
        for (i = 0; i < fpp.numOfAgain; ++i) {
            do_cmd(fpp, all_allper, all_correct, all_wrong);
//            printf("all_per : %d    correct : %d    wrong : %d\n", all_per, correct, wrong);
        }
//        printf("angin num : %d   all_allper: %d     all_correct: %d     all_wrong: %d\n",
//               numOfAgain, all_allper, all_correct, all_wrong);
        Pfn = 1.0 * (all_allper - all_correct) / all_allper;
        //printf("trace Pfn: %f\n", Pfn);
    }
    fpp.L += 50;
//    printf("-------------------\n");
//    printf("Pfn  met\n");
//    printf("-------------------\n");

    Pfp = 1.0 * all_wrong / (fpp.numOfAgain * fpp.allItemNum - all_allper);
    while (Pfp > fpp.target_fp) {
        all_allper = all_correct = all_wrong = 0;
        fpp.L += 50;
        //printf("trace L for Pfp: %d\n", fpp.L);
        for (i = 0; i < fpp.numOfAgain; ++i) {
            do_cmd(fpp, all_allper, all_correct, all_wrong);
        }
//        printf("angin num : %d   all_allper: %d     all_correct: %d     all_wrong: %d\n",
//               numOfAgain, all_allper, all_correct, all_wrong);
        Pfp = 1.0 * all_wrong / (fpp.numOfAgain * fpp.allItemNum - all_allper);
        //printf("trace Pfp: %f ", Pfp);
    }

    computeMem(fpp);

}

void printResult(FindPresistentParams &fpp, FILE *file) {
    printf("w: %d, TCAM: %d, c: %d, d: %d| L: %d, m: %d, mem: %f\n", fpp.w, fpp.tcamsize, fpp.c, fpp.d,
            fpp.L, fpp.m, fpp.mem);
    fprintf(file, "w: %d, TCAM: %d, c: %d, d: %d| L: %d, m: %d, mem: %f\n", fpp.w, fpp.tcamsize, fpp.c, fpp.d,
            fpp.L, fpp.m, fpp.mem);
}

void new_test(FindPresistentParams &fpp) {
    int all_allper = 0, all_correct = 0, all_wrong = 0;
    float min_mem = 1000000000;
    int min_L = 1000000000;
    int min_m = 1000000000;
    float mem;

    float Pfp = 1;
    float Pfn = 1;
    int i;

    for (i = 0; i < fpp.numOfAgain; ++i) {

        do_cmd(fpp, all_allper, all_correct, all_wrong);
    }
    Pfn = 1.0 * (all_allper - all_correct) / all_allper;
    if (Pfn > fpp.target_fn) {
        fpp.mem = 10000000;
        return;
    } else {
        while (Pfn <= fpp.target_fn && fpp.L > fpp.endL) {
            all_allper = all_correct = all_wrong = 0;
            fpp.L -= 50;
            //printf("trace L for Pfn: %d\n", fpp.L);
            for (i = 0; i < fpp.numOfAgain; ++i) {
                do_cmd(fpp, all_allper, all_correct, all_wrong);
//            printf("all_per : %d    correct : %d    wrong : %d\n", all_per, correct, wrong);
            }
            Pfn = 1.0 * (all_allper - all_correct) / all_allper;
            //printf("trace Pfn: %f\n", Pfn);
        }
        fpp.L += 50;
        Pfp = 1.0 * all_wrong / (fpp.numOfAgain * fpp.allItemNum - all_allper);
        while (Pfp > fpp.target_fp) {
            all_allper = all_correct = all_wrong = 0;
            fpp.L += 50;
            //printf("trace L for Pfp: %d\n", fpp.L);
            for (i = 0; i < fpp.numOfAgain; ++i) {
                do_cmd(fpp, all_allper, all_correct, all_wrong);
            }
            Pfp = 1.0 * all_wrong / (fpp.numOfAgain * fpp.allItemNum - all_allper);
//            printf("trace Pfp: %f ", Pfp);
        }
        computeMem(fpp);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        return -1;
    }

//    char * paramFile = argv[1];
//    char * dataFile = argv[2];
//    char * resFile = argv[3];
//    char * resFile2 = argv[4];
//    int startline = atoi(argv[5]);
//    char * exactfile = argv[6];
////    int N = atoi(argv[7]);
////    int T = atoi(argv[8]);
//    int is = atoi(argv[7]);
//
//    int numOfAgain = atoi(argv[8]);
//    int Pfp = atoi(argv[9]);
//    int Pfn = atoi(argv[10]);
//    int startL = atoi(argv[11]);
//    int startm = atoi(argv[12]);
    char *testParam = argv[1];


    //read params
    char buffer[50];
    char paramFile[40];
    char dataFile[40];
    char resFile[40];
    char resFile2[40];
    int startline;
    char exactFileName[40];
    int isExact;
    int numOfAgain;
    float pfp;
    float pfn;
    int startL, endL;
    int startm, endm;
    int allItemNum;
    //int tcamsize = 65;

    int n;
    int T;
    int c;
    int R;


    FILE *testParamFile = fopen(testParam, "r");
    assert(testParamFile != NULL);

    fscanf(testParamFile, "%s", buffer);
    fscanf(testParamFile, "%s", paramFile);

    fscanf(testParamFile, "%s", buffer);
    fscanf(testParamFile, "%s", dataFile);

    fscanf(testParamFile, "%s", buffer);
    fscanf(testParamFile, "%s", resFile);

    fscanf(testParamFile, "%s", buffer);
    fscanf(testParamFile, "%s", resFile2);

    fscanf(testParamFile, "%s", buffer);
    fscanf(testParamFile, "%d", &startline);

    fscanf(testParamFile, "%s", buffer);
    fscanf(testParamFile, "%s", exactFileName);

    fscanf(testParamFile, "%s", buffer);
    fscanf(testParamFile, "%d", &isExact);
//    printf("%d\n", isExact);

    fscanf(testParamFile, "%s", buffer);
    fscanf(testParamFile, "%d", &numOfAgain);

    fscanf(testParamFile, "%s", buffer);
    fscanf(testParamFile, "%f", &pfp);

    fscanf(testParamFile, "%s", buffer);
    fscanf(testParamFile, "%f", &pfn);

    fscanf(testParamFile, "%s", buffer);
    fscanf(testParamFile, "%d", &startL);

    fscanf(testParamFile, "%s", buffer);
    fscanf(testParamFile, "%d", &startm);

    fscanf(testParamFile, "%s", buffer);
    fscanf(testParamFile, "%d", &endL);

    fscanf(testParamFile, "%s", buffer);
    fscanf(testParamFile, "%d", &endm);

    fscanf(testParamFile, "%s", buffer);
    fscanf(testParamFile, "%d", &allItemNum);

    fclose(testParamFile);

    testParamFile = fopen(paramFile, "r");
    fscanf(testParamFile, "%s", buffer);
    fscanf(testParamFile, "%d", &n);

    fscanf(testParamFile, "%s", buffer);
    fscanf(testParamFile, "%d", &T);

    fscanf(testParamFile, "%s", buffer);
    fscanf(testParamFile, "%d", &c);

    fscanf(testParamFile, "%s", buffer);
    fscanf(testParamFile, "%d", &R);

    fclose(testParamFile);


    //修改参数
    int fp_fn_size = 5;
    float target_pfp[5] = {0.002, 0.004, 0.006, 0.008, 0.01};
    float target_pfn[5] = {0.001, 0.002, 0.003, 0.004, 0.005};

    int L_result[5];
    int m_result[5];
    float mem_result[5];

    int tcamsize[3] = {15, 25};
    int ts_num = 2;

    int w[2] = {T / 2, T};
    int w_num = 2;

    int c_[3] = {2, 4, 6};
    int c_num = 3;

    int d[3] = {2, 5, 8};
    int d_num = 3;


    //do cmd
//    char cmd[200];

    int L, m;
    float Pfp, Pfn;

    FILE *cmdfp;

    int i, j, o, p, q, x, y, z;

    L = 15000;
    m = 0;
    float min_mem = 1000000000;
    int min_L = 1000000000;
    int min_m = 1000000000;

    float mem;

    FindPresistentParams fpp;
    fpp.daraFile = dataFile;
    fpp.paramFile = paramFile;
    fpp.exactFileName = exactFileName;
    fpp.resFile = resFile;
    fpp.resFile2 = resFile2;
    fpp.T = T;
    fpp.N = n;
    fpp.startline = startline;
    fpp.numOfAgain = numOfAgain;
    fpp.allItemNum = allItemNum;
    fpp.endL = endL;
    fpp.R = R;



    FILE *resultfile = fopen("./testresult.txt", "w");
    assert(resultfile != NULL);

    for (i = 0; i < w_num; ++i) {   // w
        fpp.w = w[i];
        for (j = 0; j < ts_num; ++j) {   //tcam
            fpp.tcamsize = tcamsize[j];
            for (o = 0; o < c_num; ++o) {  //c
                fpp.c = c_[o];
                for (p = 0; p < d_num; ++p) {  //d
                    fpp.d = d[p];

                    fpp.L = 30000;
                    fpp.m = 0;

                    fpp.target_fn = target_pfn[0];
                    fpp.target_fp = target_pfp[0];

                    printf("====================\n");
                    printf("find range of m\n");
                    printf("====================\n");

                    test_one_time(fpp);
                    //printResult(fpp, resultfile);

                    startm = 0;
                    endm = fpp.m + 10;
                    fpp.L = 30000;
                    printf("range m: %d - %d\n", startm, endm);

                    for (q = 0; q < fp_fn_size; ++q) {             //真阳率假阳率迭代
                        fpp.target_fn = target_pfn[q];
                        fpp.target_fp = target_pfp[q];
                        printf("====================\n");
                        printf("target pfn: %f pfp: %f\n", fpp.target_fn, fpp.target_fp);
                        printf("====================\n");

                        min_mem = 1000000000;
                        min_L = 100000000;
                        min_m = 100000000;

                        fpp.L = fpp.L + 500;
                        for (x = startm; x < endm; ++x) {
                            fpp.m = x;
                            new_test(fpp);
                            printf("m: %d\n", fpp.m);
                            //computeMem(fpp);
                            if (fpp.mem < min_mem) {
                                printf("update\n");
                                min_m = fpp.m;
                                min_L = fpp.L;
                                min_mem = fpp.mem;
                            }
                        }
                        fpp.m = min_m;
                        fpp.L = min_L;
                        fpp.mem = min_mem;
                        printResult(fpp, resultfile);

                    }

                    fprintf(resultfile, "\n\n");
                }
            }
        }
    }
    fclose(resultfile);

//    pfp = target_pfp[0];
//    pfn = target_pfn[0];
////    printf("**********************\n");
////    printf("c: %d     d: %d\n", c, d);
////    printf("**********************\n");
//
//    printf("--------------------------\n");
//    printf("target: pfp : %f   pfn:%f\n", pfp, pfn);
//    printf("--------------------------\n");
//
//    if (Pfn <= pfn) {
//        while (Pfn <= pfn) {
//            all_allper = all_correct = all_wrong = 0;
//            --m;
//            printf("trace m for Pfn: %d\n", m);
//            for (i = 0; i < numOfAgain; ++i) {
//                sprintf(cmd, "./algorithm.sh %s %s %s %s %d %s %d %d %d %d %d", paramFile, dataFile, resFile, resFile2,
//                        startline, exactFileName, n, T, L, m, tcamsize);
//                cmdfp = popen(cmd, "r");
//                assert(cmdfp != NULL);
//                fscanf(cmdfp, "%d %d %d", &all_per, &correct, &wrong);
//                pclose(cmdfp);
//                all_allper += all_per;
//                all_correct += correct;
//                all_wrong += wrong;
//            }
////            printf("angin num : %d   all_allper: %d     all_correct: %d     all_wrong: %d\n",
////                   numOfAgain, all_allper, all_correct, all_wrong);
//            Pfn = 1.0 * (all_allper - all_correct) / all_allper;
////            printf("trace Pfn: %f\n", Pfn);
//        }
//        ++m;
//    } else {
//        while (Pfn > pfn) {
//            all_allper = all_correct = all_wrong = 0;
//            ++m;
//            printf("trace m for Pfn: %d\n", m);
//            for (i = 0; i < numOfAgain; ++i) {
//                sprintf(cmd, "./algorithm.sh %s %s %s %s %d %s %d %d %d %d %d", paramFile, dataFile, resFile, resFile2,
//                        startline, exactFileName, n, T, L, m, tcamsize);
//                cmdfp = popen(cmd, "r");
//                assert(cmdfp != NULL);
//                fscanf(cmdfp, "%d %d %d", &all_per, &correct, &wrong);
//                pclose(cmdfp);
//                all_allper += all_per;
//                all_correct += correct;
//                all_wrong += wrong;
//            }
////            printf("angin num : %d   all_allper: %d     all_correct: %d     all_wrong: %d\n",
////                   numOfAgain, all_allper, all_correct, all_wrong);
//            Pfn = 1.0 * (all_allper - all_correct) / all_allper;
////            printf("trace Pfn: %f\n", Pfn);
//        }
//    }
//    while (Pfn <= pfn && L > endL) {
//        all_allper = all_correct = all_wrong = 0;
//        L -= 50;
//        printf("trace L for Pfn: %d\n", L);
//        for (i = 0; i < numOfAgain; ++i) {
//            sprintf(cmd, "./algorithm.sh %s %s %s %s %d %s %d %d %d %d %d", paramFile, dataFile, resFile, resFile2,
//                    startline, exactFileName, n, T, L, m, tcamsize);
//            cmdfp = popen(cmd, "r");
//            assert(cmdfp != NULL);
//            fscanf(cmdfp, "%d %d %d", &all_per, &correct, &wrong);
//            pclose(cmdfp);
//            all_allper += all_per;
//            all_correct += correct;
//            all_wrong += wrong;
////            printf("all_per : %d    correct : %d    wrong : %d\n", all_per, correct, wrong);
//        }
////        printf("angin num : %d   all_allper: %d     all_correct: %d     all_wrong: %d\n",
////               numOfAgain, all_allper, all_correct, all_wrong);
//        Pfn = 1.0 * (all_allper - all_correct) / all_allper;
////        printf("trace Pfn: %f\n", Pfn);
//    }
//    L += 50;
////    printf("-------------------\n");
////    printf("Pfn  met\n");
////    printf("-------------------\n");
//
//    Pfp = 1.0 * all_wrong / (numOfAgain * allItemNum - all_allper);
//    while (Pfp > pfp) {
//        all_allper = all_correct = all_wrong = 0;
//        L += 50;
//        printf("trace L for Pfp: %d\n", L);
//        for (i = 0; i < numOfAgain; ++i) {
//            sprintf(cmd, "./algorithm.sh %s %s %s %s %d %s %d %d %d %d %d", paramFile, dataFile, resFile, resFile2,
//                    startline, exactFileName, n, T, L, m, tcamsize);
//            cmdfp = popen(cmd, "r");
//            assert(cmdfp != NULL);
//            fscanf(cmdfp, "%d %d %d", &all_per, &correct, &wrong);
//            pclose(cmdfp);
//            all_allper += all_per;
//            all_correct += correct;
//            all_wrong += wrong;
//        }
////        printf("angin num : %d   all_allper: %d     all_correct: %d     all_wrong: %d\n",
////               numOfAgain, all_allper, all_correct, all_wrong);
//        Pfp = 1.0 * all_wrong / (numOfAgain * allItemNum - all_allper);
//        printf("trace Pfp: %f ", Pfp);
//    }
//    printf("L : %d   m : %d  \n", L, m);
//    sprintf(cmd, "./computeMemory/a.out %d %d %d %d %d", c, R, L, m, tcamsize);
//    cmdfp = popen(cmd, "r");
//    assert(cmdfp != NULL);
//    fscanf(cmdfp, "%f", &mem);
////    printf("mem : %f\n", mem);
//    printf("*******************************************\n");
//    printf("min_L : %d   min_m: %d  min_mem: %f \n", L, m, mem);
//    printf("*******************************************\n");
//    L_result[0] = L;
//    m_result[0] = m;
//    mem_result[0] = mem;
//
//
//    int start_m = 1;
//    int end_m = m + 20;
//    for (int pp = 1; pp < fp_fn_size; ++pp) {
//        pfp = target_pfp[pp];
//        pfn = target_pfn[pp];
//        printf("--------------------------\n");
//        printf("target: pfp : %f   pfn:%f\n", pfp, pfn);
//        printf("--------------------------\n");
//
//
//        min_mem = 1000000000;
//        min_L = 100000000;
//        min_m = 100000000;
//
//        for (m = start_m; m < end_m - 1; ++m) {
//            Pfn = 1;
//            Pfp = 1;
//
//            m--;
//            if (Pfn <= pfn) {
//                while (Pfn <= pfn) {
//                    all_allper = all_correct = all_wrong = 0;
//                    --m;
//                    printf("trace m for Pfn: %d\n", m);
//                    for (i = 0; i < numOfAgain; ++i) {
//                        sprintf(cmd, "./algorithm.sh %s %s %s %s %d %s %d %d %d %d %d", paramFile, dataFile, resFile,
//                                resFile2,
//                                startline, exactFileName, n, T, L, m, tcamsize);
//                        cmdfp = popen(cmd, "r");
//                        assert(cmdfp != NULL);
//                        fscanf(cmdfp, "%d %d %d", &all_per, &correct, &wrong);
//                        pclose(cmdfp);
//                        all_allper += all_per;
//                        all_correct += correct;
//                        all_wrong += wrong;
//                    }
////                    printf("angin num : %d   all_allper: %d     all_correct: %d     all_wrong: %d\n",
////                           numOfAgain, all_allper, all_correct, all_wrong);
//                    Pfn = 1.0 * (all_allper - all_correct) / all_allper;
////                    printf("trace Pfn: %f\n", Pfn);
//                }
//                ++m;
//            } else {
//                while (Pfn > pfn) {
//                    all_allper = all_correct = all_wrong = 0;
//                    ++m;
//                    printf("trace m for Pfn: %d\n", m);
//                    for (i = 0; i < numOfAgain; ++i) {
//                        sprintf(cmd, "./algorithm.sh %s %s %s %s %d %s %d %d %d %d %d", paramFile, dataFile, resFile,
//                                resFile2,
//                                startline, exactFileName, n, T, L, m, tcamsize);
//                        cmdfp = popen(cmd, "r");
//                        assert(cmdfp != NULL);
//                        fscanf(cmdfp, "%d %d %d", &all_per, &correct, &wrong);
//                        pclose(cmdfp);
//                        all_allper += all_per;
//                        all_correct += correct;
//                        all_wrong += wrong;
//                    }
////                    printf("angin num : %d   all_allper: %d     all_correct: %d     all_wrong: %d\n",
////                           numOfAgain, all_allper, all_correct, all_wrong);
//                    Pfn = 1.0 * (all_allper - all_correct) / all_allper;
////                    printf("trace Pfn: %f\n", Pfn);
//                }
//            }
//
//            while (Pfn <= pfn && L > endL) {
//                all_allper = all_correct = all_wrong = 0;
//                L -= 50;
//                printf("trace L for Pfn: %d\n", L);
//                for (i = 0; i < numOfAgain; ++i) {
//                    sprintf(cmd, "./algorithm.sh %s %s %s %s %d %s %d %d %d %d %d", paramFile, dataFile, resFile,
//                            resFile2,
//                            startline, exactFileName, n, T, L, m, tcamsize);
//                    cmdfp = popen(cmd, "r");
//                    assert(cmdfp != NULL);
//                    fscanf(cmdfp, "%d %d %d", &all_per, &correct, &wrong);
//                    pclose(cmdfp);
//                    all_allper += all_per;
//                    all_correct += correct;
//                    all_wrong += wrong;
////            printf("all_per : %d    correct : %d    wrong : %d\n", all_per, correct, wrong);
//                }
////                printf("angin num : %d   all_allper: %d     all_correct: %d     all_wrong: %d\n",
////                       numOfAgain, all_allper, all_correct, all_wrong);
//                Pfn = 1.0 * (all_allper - all_correct) / all_allper;
////                printf("trace Pfn: %f\n", Pfn);
//            }
//            L += 50;
//
////            printf("-------------------\n");
////            printf("Pfn  met\n");
////            printf("-------------------\n");
//
//            Pfp = 1.0 * all_wrong / (numOfAgain * allItemNum - all_allper);
//
//
//            while (Pfp > pfp) {
//                all_allper = all_correct = all_wrong = 0;
//                L += 50;
////                printf("trace L for Pfp: %d\n", L);
//                for (i = 0; i < numOfAgain; ++i) {
//                    sprintf(cmd, "./algorithm.sh %s %s %s %s %d %s %d %d %d %d %d", paramFile, dataFile, resFile,
//                            resFile2,
//                            startline, exactFileName, n, T, L, m, tcamsize);
//                    cmdfp = popen(cmd, "r");
//                    assert(cmdfp != NULL);
//                    fscanf(cmdfp, "%d %d %d", &all_per, &correct, &wrong);
//                    pclose(cmdfp);
//                    all_allper += all_per;
//                    all_correct += correct;
//                    all_wrong += wrong;
//                }
////                printf("angin num : %d   all_allper: %d     all_correct: %d     all_wrong: %d\n",
////                       numOfAgain, all_allper, all_correct, all_wrong);
//                Pfp = 1.0 * all_wrong / (numOfAgain * allItemNum - all_allper);
////                printf("trace Pfp: %f\n", Pfp);
//            }
//            printf("L : %d   m : %d  ", L, m);
//            sprintf(cmd, "./computeMemory/a.out %d %d %d %d %d", c, R, L, m, tcamsize);
//            cmdfp = popen(cmd, "r");
//            assert(cmdfp != NULL);
//            fscanf(cmdfp, "%f", &mem);
//            printf("mem : %f\n", mem);
//            if (mem < min_mem) {
//                printf("update\n");
//                min_mem = mem;
//                min_L = L;
//                min_m = m;
//            }
//        }
//        printf("*************************************\n");
//        printf("min_L : %d   min_m: %d  min_mem: %f \n", min_L, min_m, min_mem);
//        printf("*************************************\n");
//        L_result[pp] = min_L;
//        m_result[pp] = min_m;
//        mem_result[pp] = min_mem;
//        L = min_L + 1000 < 30000 ? min_L + 1000 : 30000;
//    }
//    printf("=============================================\n");
//    for (int i = 0; i < fp_fn_size; ++i) {
//        printf("L: %d    m: %d    mem: %f\n", L_result[i], m_result[i], mem_result[i]);
//    }
//
//
////    printf("trace m : %d\n", m);
////    sprintf(cmd, "./algorithm.sh %s %s %s %s %d %s %d %d %d %d", paramFile, dataFile, resFile, resFile2,
////            startline, exactFileName, n, T, L, m);
////    cmdfp = popen(cmd, "r");
////    assert(cmdfp != NULL);
////    fscanf(cmdfp, "%d %d %d", &all_per, &correct, &wrong);
////    pclose(cmdfp);
////    printf("all_per : %d    correct : %d    wrong : %d\n", all_per, correct, wrong);
////    Pfn = 1.0 *(all_per - correct) / all_per;
////    printf("trace Pfn: %f\n", Pfn);
////
////    if (Pfn > pfn) {
////        while (Pfn > pfn) {
////            L += 100;
////            printf("trace L  : %d\n", L);
////            sprintf(cmd, "./algorithm.sh %s %s %s %s %d %s %d %d %d %d", paramFile, dataFile, resFile, resFile2,
////                    startline, exactFileName, n, T, L, m);
////            cmdfp = popen(cmd, "r");
////            assert(cmdfp != NULL);
////            fscanf(cmdfp, "%d %d %d", &all_per, &correct, &wrong);
////            pclose(cmdfp);
////            printf("all_per : %d    correct : %d    wrong : %d\n", all_per, correct, wrong);
////            Pfn = 1.0 *(all_per - correct) / all_per;
////            printf("trace Pfn: %f\n", Pfn);
////        }
////
////    }
////    while (Pfn <= pfn) {
////        --m;
////        printf("trace m : %d\n", m);
////        sprintf(cmd, "./algorithm.sh %s %s %s %s %d %s %d %d %d %d", paramFile, dataFile, resFile, resFile2,
////                startline, exactFileName, n, T, L, m);
////        cmdfp = popen(cmd, "r");
////        assert(cmdfp != NULL);
////        fscanf(cmdfp, "%d %d %d", &all_per, &correct, &wrong);
////        pclose(cmdfp);
////        printf("all_per : %d    correct : %d    wrong : %d\n", all_per, correct, wrong);
////        Pfn = 1.0 *(all_per - correct) / all_per;
////        printf("trace Pfn: %f\n", Pfn);
////    }
////    ++m;
//
//
////    printf("L : %d   m : %d  ", L, m);
////    sprintf(cmd, "./computeMemory/a.out %d %d %d %d", c, R, L, m);
////    cmdfp = popen(cmd, "r");
////    assert(cmdfp != NULL);
////    fscanf(cmdfp, "%f", &mem);
////    printf("mem : %f\n", mem);
//
//
//

    return 0;
}
