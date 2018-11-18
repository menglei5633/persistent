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
    int k;      //Number of BloomFilter's hash functions
    int L;      //Length of BloomFilter: B and b
    int c;      //Number of cells in each bucket
    int dimission;      //dimission of item
    int d;      //Number of candidate buckets for an item
    int m;      //Number of buckets in the SRAM
    int R;      //Memory space to stock an item
    int isReconstruction;       //weither to Reconstruction after T epoch
    int findNum;        //the number of item to find
    int TCAMSize;   //the size of tcam

};

void readParam(FILE *fp, FindPresistentParams *fpp) {
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

int main(int argc, char* argv[]) {
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
    char* testParam = argv[1];
    int fp_fn_size = 1;
//    float target_pfp[5] = {0.002, 0.004, 0.006, 0.008, 0.01};
    float target_pfp[1] = {0.002};
//    float target_pfn[5] = {0.001, 0.002, 0.003, 0.004, 0.005};
    float target_pfn[1] = {0.001};
//    int L_result[5];
//    int m_result[5];
//    float mem_result[5];

    int L_result[1];
    int m_result[1];
    float mem_result[1];

    FILE* testParamFile = fopen(testParam, "r");
    assert(testParamFile != NULL);

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
    int tcamsize = 5;
    int stepOfTACM = 20;
    int tcamsizeend = 205;

    int n;
    int T;
    int c;
    int R;

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


//    FindPresistentParams fpp;
//    readParam(testParamFile, &fpp);

    fclose(testParamFile);

//    printf("%d\n", fpp.TCAMSize);
    //read param end

    //do cmd
    char cmd[200];
    int all_per;
    int correct;
    int wrong;
    int L, m;
    float Pfp, Pfn;
    L = startL;
    m = startm;
    Pfp = 1;
    Pfn = 1;
    FILE * cmdfp;
    int all_allper, all_correct, all_wrong;
    int i;

    for (int ts = tcamsize; ts < tcamsizeend; ts+=stepOfTACM) {
        printf("++++++++++tcamsize: %d\n", ts);
        L = 30000;
        m = 1;
        float min_mem = 1000000000;
        int min_L = 1000000000;
        int min_m = 1000000000;
        float mem;

        pfp = target_pfp[0];
        pfn = target_pfn[0];
//    printf("**********************\n");
//    printf("c: %d     d: %d\n", c, d);
//    printf("**********************\n");

        printf("--------------------------\n");
        printf("target: pfp : %f   pfn:%f\n", pfp, pfn);
        printf("--------------------------\n");

        if (Pfn <= pfn) {
            while (Pfn <= pfn) {
                all_allper = all_correct = all_wrong = 0;
                --m;
                printf("trace m for Pfn: %d\n", m);
                for (i = 0; i < numOfAgain; ++i) {
                    sprintf(cmd, "./algorithm.sh %s %s %s %s %d %s %d %d %d %d %d", paramFile, dataFile, resFile, resFile2,
                            startline, exactFileName, n, T, L, m, ts);
                    cmdfp = popen(cmd, "r");
                    assert(cmdfp != NULL);
                    fscanf(cmdfp, "%d %d %d", &all_per, &correct, &wrong);
                    pclose(cmdfp);
                    all_allper += all_per;
                    all_correct += correct;
                    all_wrong += wrong;
                }
//            printf("angin num : %d   all_allper: %d     all_correct: %d     all_wrong: %d\n",
//                   numOfAgain, all_allper, all_correct, all_wrong);
                Pfn = 1.0 * (all_allper - all_correct) / all_allper;
//            printf("trace Pfn: %f\n", Pfn);
            }
            ++m;
        } else {
            while (Pfn > pfn) {
                all_allper = all_correct = all_wrong = 0;
                ++m;
                printf("trace m for Pfn: %d\n", m);
                for (i = 0; i < numOfAgain; ++i) {
                    sprintf(cmd, "./algorithm.sh %s %s %s %s %d %s %d %d %d %d %d", paramFile, dataFile, resFile, resFile2,
                            startline, exactFileName, n, T, L, m, ts);
                    cmdfp = popen(cmd, "r");
                    assert(cmdfp != NULL);
                    fscanf(cmdfp, "%d %d %d", &all_per, &correct, &wrong);
                    pclose(cmdfp);
                    all_allper += all_per;
                    all_correct += correct;
                    all_wrong += wrong;
                }
//            printf("angin num : %d   all_allper: %d     all_correct: %d     all_wrong: %d\n",
//                   numOfAgain, all_allper, all_correct, all_wrong);
                Pfn = 1.0 * (all_allper - all_correct) / all_allper;
//            printf("trace Pfn: %f\n", Pfn);
            }
        }
        while (Pfn <= pfn && L > endL) {
            all_allper = all_correct = all_wrong = 0;
            L -= 50;
            printf("trace L for Pfn: %d\n", L);
            for (i = 0; i < numOfAgain; ++i) {
                sprintf(cmd, "./algorithm.sh %s %s %s %s %d %s %d %d %d %d %d", paramFile, dataFile, resFile, resFile2,
                        startline, exactFileName, n, T, L, m, ts);
                cmdfp = popen(cmd, "r");
                assert(cmdfp != NULL);
                fscanf(cmdfp, "%d %d %d", &all_per, &correct, &wrong);
                pclose(cmdfp);
                all_allper += all_per;
                all_correct += correct;
                all_wrong += wrong;
//            printf("all_per : %d    correct : %d    wrong : %d\n", all_per, correct, wrong);
            }
//        printf("angin num : %d   all_allper: %d     all_correct: %d     all_wrong: %d\n",
//               numOfAgain, all_allper, all_correct, all_wrong);
            Pfn = 1.0 * (all_allper - all_correct) / all_allper;
//        printf("trace Pfn: %f\n", Pfn);
        }
        L += 50;
//    printf("-------------------\n");
//    printf("Pfn  met\n");
//    printf("-------------------\n");

        Pfp = 1.0 * all_wrong / (numOfAgain * allItemNum - all_allper);
        while (Pfp > pfp) {
            all_allper = all_correct = all_wrong = 0;
            L += 50;
            printf("trace L for Pfp: %d\n", L);
            for (i = 0; i < numOfAgain; ++i) {
                sprintf(cmd, "./algorithm.sh %s %s %s %s %d %s %d %d %d %d %d", paramFile, dataFile, resFile, resFile2,
                        startline, exactFileName, n, T, L, m, ts);
                cmdfp = popen(cmd, "r");
                assert(cmdfp != NULL);
                fscanf(cmdfp, "%d %d %d", &all_per, &correct, &wrong);
                pclose(cmdfp);
                all_allper += all_per;
                all_correct += correct;
                all_wrong += wrong;
            }
//        printf("angin num : %d   all_allper: %d     all_correct: %d     all_wrong: %d\n",
//               numOfAgain, all_allper, all_correct, all_wrong);
            Pfp = 1.0 * all_wrong / (numOfAgain * allItemNum - all_allper);
            printf("trace Pfp: %f ", Pfp);
        }
        printf("L : %d   m : %d  \n", L, m);
        sprintf(cmd, "./computeMemory/a.out %d %d %d %d %d", c, R, L, m, tcamsize);
        cmdfp = popen(cmd, "r");
        assert(cmdfp != NULL);
        fscanf(cmdfp, "%f", &mem);
//    printf("mem : %f\n", mem);
        printf("*******************************************\n");
        printf("min_L : %d   min_m: %d  min_mem: %f \n", L, m, mem);
        printf("*******************************************\n");
        L_result[0] = L;
        m_result[0] = m;
        mem_result[0] = mem;


        int start_m = 1;
        int end_m = m + 40;
        for (int pp = 0; pp < fp_fn_size; ++pp) {
            pfp = target_pfp[pp];
            pfn = target_pfn[pp];
            printf("--------------------------\n");
            printf("target: pfp : %f   pfn:%f\n", pfp, pfn);
            printf("--------------------------\n");


            min_mem = 1000000000;
            min_L = 100000000;
            min_m = 100000000;

            for (m = start_m; m < end_m - 1; ++m) {
                Pfn = 1;
                Pfp = 1;

                m--;
                if (Pfn <= pfn) {
                    while (Pfn <= pfn) {
                        all_allper = all_correct = all_wrong = 0;
                        --m;
                        printf("trace m for Pfn: %d\n", m);
                        for (i = 0; i < numOfAgain; ++i) {
                            sprintf(cmd, "./algorithm.sh %s %s %s %s %d %s %d %d %d %d %d", paramFile, dataFile, resFile,
                                    resFile2, startline, exactFileName, n, T, L, m, ts);
                            cmdfp = popen(cmd, "r");
                            assert(cmdfp != NULL);
                            fscanf(cmdfp, "%d %d %d", &all_per, &correct, &wrong);
                            pclose(cmdfp);
                            all_allper += all_per;
                            all_correct += correct;
                            all_wrong += wrong;
                        }
//                    printf("angin num : %d   all_allper: %d     all_correct: %d     all_wrong: %d\n",
//                           numOfAgain, all_allper, all_correct, all_wrong);
                        Pfn = 1.0 * (all_allper - all_correct) / all_allper;
//                    printf("trace Pfn: %f\n", Pfn);
                    }
                    ++m;
                } else {
                    while (Pfn > pfn) {
                        all_allper = all_correct = all_wrong = 0;
                        ++m;
                        printf("trace m for Pfn: %d\n", m);
                        for (i = 0; i < numOfAgain; ++i) {
                            sprintf(cmd, "./algorithm.sh %s %s %s %s %d %s %d %d %d %d %d", paramFile, dataFile, resFile,
                                    resFile2,
                                    startline, exactFileName, n, T, L, m, ts);
                            cmdfp = popen(cmd, "r");
                            assert(cmdfp != NULL);
                            fscanf(cmdfp, "%d %d %d", &all_per, &correct, &wrong);
                            pclose(cmdfp);
                            all_allper += all_per;
                            all_correct += correct;
                            all_wrong += wrong;
                        }
//                    printf("angin num : %d   all_allper: %d     all_correct: %d     all_wrong: %d\n",
//                           numOfAgain, all_allper, all_correct, all_wrong);
                        Pfn = 1.0 * (all_allper - all_correct) / all_allper;
//                    printf("trace Pfn: %f\n", Pfn);
                    }
                }

                while (Pfn <= pfn && L > endL) {
                    all_allper = all_correct = all_wrong = 0;
                    L -= 50;
                    printf("trace L for Pfn: %d\n", L);
                    for (i = 0; i < numOfAgain; ++i) {
                        sprintf(cmd, "./algorithm.sh %s %s %s %s %d %s %d %d %d %d %d", paramFile, dataFile, resFile,
                                resFile2,
                                startline, exactFileName, n, T, L, m, ts);
                        cmdfp = popen(cmd, "r");
                        assert(cmdfp != NULL);
                        fscanf(cmdfp, "%d %d %d", &all_per, &correct, &wrong);
                        pclose(cmdfp);
                        all_allper += all_per;
                        all_correct += correct;
                        all_wrong += wrong;
//            printf("all_per : %d    correct : %d    wrong : %d\n", all_per, correct, wrong);
                    }
//                printf("angin num : %d   all_allper: %d     all_correct: %d     all_wrong: %d\n",
//                       numOfAgain, all_allper, all_correct, all_wrong);
                    Pfn = 1.0 * (all_allper - all_correct) / all_allper;
//                printf("trace Pfn: %f\n", Pfn);
                }
                L += 50;

//            printf("-------------------\n");
//            printf("Pfn  met\n");
//            printf("-------------------\n");

                Pfp = 1.0 * all_wrong / (numOfAgain * allItemNum - all_allper);


                while (Pfp > pfp) {
                    all_allper = all_correct = all_wrong = 0;
                    L += 50;
//                printf("trace L for Pfp: %d\n", L);
                    for (i = 0; i < numOfAgain; ++i) {
                        sprintf(cmd, "./algorithm.sh %s %s %s %s %d %s %d %d %d %d %d", paramFile, dataFile, resFile,
                                resFile2,
                                startline, exactFileName, n, T, L, m, ts);
                        cmdfp = popen(cmd, "r");
                        assert(cmdfp != NULL);
                        fscanf(cmdfp, "%d %d %d", &all_per, &correct, &wrong);
                        pclose(cmdfp);
                        all_allper += all_per;
                        all_correct += correct;
                        all_wrong += wrong;
                    }
//                printf("angin num : %d   all_allper: %d     all_correct: %d     all_wrong: %d\n",
//                       numOfAgain, all_allper, all_correct, all_wrong);
                    Pfp = 1.0 * all_wrong / (numOfAgain * allItemNum - all_allper);
//                printf("trace Pfp: %f\n", Pfp);
                }
                printf("L : %d   m : %d  ", L, m);
                sprintf(cmd, "./computeMemory/a.out %d %d %d %d %d", c, R, L, m, tcamsize);
                cmdfp = popen(cmd, "r");
                assert(cmdfp != NULL);
                fscanf(cmdfp, "%f", &mem);
                printf("mem : %f\n", mem);
                if (mem < min_mem) {
                    printf("update\n");
                    min_mem = mem;
                    min_L = L;
                    min_m = m;
                }
            }
            printf("*************************************\n");
            printf("min_L : %d   min_m: %d  min_mem: %f \n", min_L, min_m, min_mem);
            printf("*************************************\n");
            L_result[pp] = min_L;
            m_result[pp] = min_m;
            mem_result[pp] = min_mem;
            L = min_L + 1000 < 30000 ? min_L + 1000 : 30000;
        }
        printf("=============================================\n");
        for (int i = 0; i < fp_fn_size; ++i) {
            printf("L: %d    m: %d    mem: %f\n", L_result[i], m_result[i], mem_result[i]);
        }
    }


//    printf("trace m : %d\n", m);
//    sprintf(cmd, "./algorithm.sh %s %s %s %s %d %s %d %d %d %d", paramFile, dataFile, resFile, resFile2,
//            startline, exactFileName, n, T, L, m);
//    cmdfp = popen(cmd, "r");
//    assert(cmdfp != NULL);
//    fscanf(cmdfp, "%d %d %d", &all_per, &correct, &wrong);
//    pclose(cmdfp);
//    printf("all_per : %d    correct : %d    wrong : %d\n", all_per, correct, wrong);
//    Pfn = 1.0 *(all_per - correct) / all_per;
//    printf("trace Pfn: %f\n", Pfn);
//
//    if (Pfn > pfn) {
//        while (Pfn > pfn) {
//            L += 100;
//            printf("trace L  : %d\n", L);
//            sprintf(cmd, "./algorithm.sh %s %s %s %s %d %s %d %d %d %d", paramFile, dataFile, resFile, resFile2,
//                    startline, exactFileName, n, T, L, m);
//            cmdfp = popen(cmd, "r");
//            assert(cmdfp != NULL);
//            fscanf(cmdfp, "%d %d %d", &all_per, &correct, &wrong);
//            pclose(cmdfp);
//            printf("all_per : %d    correct : %d    wrong : %d\n", all_per, correct, wrong);
//            Pfn = 1.0 *(all_per - correct) / all_per;
//            printf("trace Pfn: %f\n", Pfn);
//        }
//
//    }
//    while (Pfn <= pfn) {
//        --m;
//        printf("trace m : %d\n", m);
//        sprintf(cmd, "./algorithm.sh %s %s %s %s %d %s %d %d %d %d", paramFile, dataFile, resFile, resFile2,
//                startline, exactFileName, n, T, L, m);
//        cmdfp = popen(cmd, "r");
//        assert(cmdfp != NULL);
//        fscanf(cmdfp, "%d %d %d", &all_per, &correct, &wrong);
//        pclose(cmdfp);
//        printf("all_per : %d    correct : %d    wrong : %d\n", all_per, correct, wrong);
//        Pfn = 1.0 *(all_per - correct) / all_per;
//        printf("trace Pfn: %f\n", Pfn);
//    }
//    ++m;


//    printf("L : %d   m : %d  ", L, m);
//    sprintf(cmd, "./computeMemory/a.out %d %d %d %d", c, R, L, m);
//    cmdfp = popen(cmd, "r");
//    assert(cmdfp != NULL);
//    fscanf(cmdfp, "%f", &mem);
//    printf("mem : %f\n", mem);




    return 0;
}
