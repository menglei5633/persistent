//
// Created by lm on 18-9-3.
//
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <vector>
#include <string>
#include <set>
#include <thread>
#include <iostream>
#include "PresistentArgorithm/FindPresistent.h"
#include "BloomFilter/HashFunction.h"
#include "FileOption/FileOption.h"

//int HashFunctionFactory::num = 0;
struct TestResult
{
    int w;
    int tcamsize;
    int m;
    int L;
    float mem;
    float target_pfn;
    float target_pfp;
};

struct TestParam
{
    FindPresistentParams fpp;
    int again_time;
    std::string data_file;
    std::string exact_file;
    std::string output_dir;
    int start_line;
    int all_num;
};

struct TestOneParam
{
    int again_time;
    int all_num;
    float target_pfn;
    float target_pfp;
};

struct RunParam
{
    FindPresistentParams fpp;
    std::string data_file;
    // std::string output_dir;
    int start_line;
    std::set<std::string> persistent_exact;
};

struct RunResult
{
    int wrong;
    int correct;
    int per_num;
};

void tongji(std::set<std::string> &persistent_all, std::set<std::string> &persistent_insert,
            std::set<std::string> &persistent_exact, int &wrong, int &correct)
{
    // std::set<std::string>::iterator it;
    int all_correct = persistent_exact.size();
    wrong = 0;
    correct = 0;
    for (std::string str : persistent_all)
    {
        if (persistent_exact.find(str) == persistent_exact.end())
        {
            wrong++;
        }
    }
    for (std::string str : persistent_insert)
    {
        if (persistent_exact.find(str) != persistent_exact.end())
        {
            correct++;
        }
    }
}

void runFindPresistent(RunParam &rp, RunResult &rr)
{
    FindPresistent fp = FindPresistent(&rp.fpp);
    FILE *data_fp = fopen(rp.data_file.c_str(), "r");
    int i = 0;
    char input[100];
    for (i = 0; i < rp.start_line; ++i)
    {
        DataFileOptions::readOneLine(data_fp, input);
    }
    for (i = 0; i < rp.fpp.N * rp.fpp.T; ++i)
    {
        DataFileOptions::readOneLine(data_fp, input);
        fp.receive_a_item(input);
    }
    if (rp.fpp.isReconstruction)
    {
        fp.list_reconstruction();
    }
    tongji(fp.persistents_all, fp.persistents_insert, rp.persistent_exact, rr.wrong, rr.correct);
    // fclose(file2_fp);
    fclose(data_fp);
}

inline float getPfn(int all_per, int all_correct)
{
    return (1.0 * (all_per - all_correct) / all_per);
}

inline float getPfp(int all_wrong, int all_num_times, int all_per)
{
    return (1.0 * all_wrong / (all_num_times - all_per));
}

inline float getMem(FindPresistentParams fpp)
{
    float mem = 1.0 * (fpp.L + fpp.m * fpp.R * fpp.c * 8 + fpp.TCAMSize * fpp.R * 8) / 8;
    return mem;
}

float testOneTime(TestOneParam &top, RunParam &rp)
{
    int all_per = 0, all_correct = 0, all_wrong = 0;
    float pfn = 1;
    float pfp = 1;
    float min_mem = 1000000000;
    int min_L = 1000000000;
    int min_m = 1000000000;
    all_per = all_correct = all_wrong = 0;
    RunResult rr;
    for (int i = 0; i < top.again_time; ++i)
    {
        rr.correct = rr.wrong = rr.per_num = 0;
        runFindPresistent(rp, rr);
        all_per += rr.per_num;
        all_correct += rr.correct;
        all_wrong += rr.wrong;
    }
    pfn = getPfn(all_per, all_correct);
    if (pfn < top.target_pfn)
    {
        while (pfn < top.target_pfn && rp.fpp.m > 0)
        {
            all_per = all_correct = all_wrong = 0;
            --rp.fpp.m;
            for (int i = 0; i < top.again_time; ++i)
            {
                rr.correct = rr.wrong = rr.per_num = 0;
                runFindPresistent(rp, rr);
                all_per += rr.per_num;
                all_correct += rr.correct;
                all_wrong += rr.wrong;
            }
            pfn = getPfn(all_per, all_correct);
        }
        ++rp.fpp.m;
    }
    else
    {
        while (pfn > top.target_pfn)
        {
            all_per = all_correct = all_wrong = 0;
            ++rp.fpp.m;
            for (int i = 0; i < top.again_time; ++i)
            {
                rr.correct = rr.wrong = rr.per_num = 0;
                runFindPresistent(rp, rr);
                all_per += rr.per_num;
                all_correct += rr.correct;
                all_wrong += rr.wrong;
            }
            pfn = getPfn(all_per, all_correct);
        }
    }
    while (pfn <= top.target_pfn && rp.fpp.L > 5000)
    {
        rp.fpp.L -= 100;
        all_per = all_correct = all_wrong = 0;
        for (int i = 0; i < top.again_time; ++i)
        {
            rr.correct = rr.wrong = rr.per_num = 0;
            runFindPresistent(rp, rr);
            all_per += rr.per_num;
            all_correct += rr.correct;
            all_wrong += rr.wrong;
        }
        pfn = getPfn(all_per, all_correct);
    }
    rp.fpp.L += 100;
    pfp = getPfp(all_wrong, top.again_time * top.all_num, all_per);
    while (pfp > top.target_pfp)
    {
        rp.fpp.L += 100;
        all_per = all_correct = all_wrong = 0;
        for (int i = 0; i < top.again_time; ++i)
        {
            rr.correct = rr.wrong = rr.per_num = 0;
            runFindPresistent(rp, rr);
            all_per += rr.per_num;
            all_correct += rr.correct;
            all_wrong += rr.wrong;
        }
        pfp = getPfp(all_wrong, top.again_time * top.all_num, all_per);
    }
    return getMem(rp.fpp);
}

float new_test(TestOneParam &top, RunParam &rp)
{
    int all_per = 0, all_correct = 0, all_wrong = 0;
    float pfn = 1;
    float pfp = 1;
    float min_mem = 1000000000;
    int min_L = 1000000000;
    int min_m = 1000000000;
    all_per = all_correct = all_wrong = 0;
    RunResult rr;
    for (int i = 0; i < top.again_time; ++i)
    {
        rr.correct = rr.wrong = rr.per_num = 0;
        runFindPresistent(rp, rr);
        all_per += rr.per_num;
        all_correct += rr.correct;
        all_wrong += rr.wrong;
    }
    pfn = getPfn(all_per, all_correct);
    if (pfn > top.target_pfn)
    {
        return 100000000;
    }
    else
    {
        while (pfn < top.target_pfn && rp.fpp.L > 5000)
        {
            rp.fpp.L -= 100;
            all_per = all_correct = all_wrong = 0;
            for (int i = 0; i < top.again_time; ++i)
            {
                rr.correct = rr.wrong = rr.per_num = 0;
                runFindPresistent(rp, rr);
                all_per += rr.per_num;
                all_correct += rr.correct;
                all_wrong += rr.wrong;
            }
            pfn = getPfn(all_per, all_correct);
        }
        rp.fpp.L += 100;
        pfp = getPfp(all_wrong, top.again_time * top.all_num, all_per);
        while (pfp > top.target_pfp)
        {
            rp.fpp.L += 100;
            all_per = all_correct = all_wrong = 0;
            for (int i = 0; i < top.again_time; ++i)
            {
                rr.correct = rr.wrong = rr.per_num = 0;
                runFindPresistent(rp, rr);
                all_per += rr.per_num;
                all_correct += rr.correct;
                all_wrong += rr.wrong;
            }
            pfp = getPfp(all_wrong, top.again_time * top.all_num, all_per);
        }
        return getMem(rp.fpp);
    }
}

inline void recordToFile(FindPresistentParams fpp, float mem)
{
    fprintf(fp, "w:%d,TCAMSize:%d,c:%d,d:%d|L:%d,m:%d,mem:%f\n", fpp.w, fpp.TCAMSize,
            fpp.c, fpp.d, fpp.L, fpp.m, mem);
}

void runTest(TestParam tp)
{
    float target_pfn[] = {0.001, 0.002, 0.003, 0.004, 0.005};
    float target_pfp[] = {0.002, 0.004, 0.006, 0.008, 0.01};
    int target_num = 5;
    RunParam rp;
    rp.data_file = tp.data_file;
    rp.start_line = tp.start_line;
    FILE *exact_fp = fopen(tp.exact_file.c_str(), "r");
    assert(exact_fp != NULL);
    char buffer[100];
    while (fscanf(exact_fp, "%s", buffer) > 0)
    {
        rp.persistent_exact.insert(std::string(buffer));
    }
    fclose(exact_fp);
    rp.fpp = tp.fpp;
    TestOneParam top;
    std::string record_file_name = tp.output_dir + "/w" + std::to_string(tp.fpp.w) + "_ts" + std::to_string(tp.fpp.TCAMSize)
                            +"_c"+std::to_string(tp.fpp.c)+"_d"+tp.fpp.d+".txt";
    FILE *record_fp = fopen(record_file_name.c_str(), "w");
    assert(record_fp != NULL);

    //w,tcansize,c,都是确定的
    float t_pfn,
        t_pfp;
    int start_m = 0, end_m;

    int min_mem = 1000000000;
    int min_L = 100000000;
    int min_m = 100000000;

    rp.fpp.L = 30000;
    rp.fpp.m = 0;

    top.target_pfn = target_pfn[0];
    top.target_pfp = target_pfp[0];
    top.again_time = tp.again_time;
    top.all_num = tp.all_num;

    float mem = testOneTime(top, rp);
    end_m = rp.fpp.m + 10; // 有待确定
    if (mem < min_mem)
    {
        min_mem = mem;
        min_L = rp.fpp.L;
        min_m = rp.fpp.m;
    }
    while (rp.fpp.m < end_m)
    {
        rp.fpp.m += 1;
        mem = new_test(top, rp);
        if (mem < min_mem)
        {
            min_mem = mem;
            min_L = rp.fpp.L;
            min_m = rp.fpp.m;
        }
    }
    rp.fpp.L = min_L;
    rp.fpp.m = min_m;
    recordToFile(rp.fpp, min_mem);
    for (int i = 1; i < target_num; ++i)
    {
        min_mem = 1000000000;
        min_L = 100000000;
        min_m = 100000000;
        top.target_pfn = target_pfn[i];
        top.target_pfp = target_pfp[i];
        rp.fpp.L += 500;
        for (rp.fpp.m = start_m; rp.fpp.m < end_m; rp.fpp.m++) {
            mem = new_test(top, rp);
            if (mem < min_mem)
            {
                min_mem = mem;
                min_L = rp.fpp.L;
                min_m = rp.fpp.m;
            }
        }
        rp.fpp.L = min_L;
        rp.fpp.m = min_m;
        recordToFile(rp.fpp, min_mem);
    }
    fclose(record_fp);
}

int main(int argc, char *argv[])
{
    if (argc < 8) {
        std::cout<<"param error..."<<std::endl;        
        return -1;
    }
    std::cout<<"start..."<<std::endl;
    TestParam tp;
    int w = atoi(argv[1]);
    int tcamsize = atoi(argv[2]);
    tp.data_file = std::string(argv[3]);
    tp.exact_file = std::string(argv[4]);
    tp.output_dir = std::string(argv[5]);
    std::string param_file_name = std::string(argv[6]);    
    tp.all_num = atoi(argv[7]);
    tp.start_line = atoi(argv[8]);
    
    DataFileOptions::readParam(param_file_name, &tp.fpp);
    tp.fpp.w = w;
    tp.fpp.TCAMSize = tcamsize;
    int c[] = {2, 4, 6};
    int d[] = {2, 5, 8};
    int c_num = 3;
    int d_num = 3;
    for (int i = 0; i < c_num; ++i) {
        for (int j = 0; j < d_num; ++j) {
            tp.fpp.c = c[i];
            tp.fpp.d = d[j];
            std::thread t(runTest, tp);
            // t.detach();
            t.join();
        }
    }
}
