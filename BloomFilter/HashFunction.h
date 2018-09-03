//
// Created by lm on 18-9-1.
//

#ifndef SRC_HASHFUNCTION_H
#define SRC_HASHFUNCTION_H


class HashFunction {
public:
    virtual unsigned int hash(char * data) = 0;
    virtual unsigned int hash(int * data) = 0;
};

class BasicHashFunction: public HashFunction {
public:
    unsigned int* a;
    unsigned int b;
    unsigned int dimission;
public:
    unsigned int hash(char * data);
    unsigned int hash(int * data);
    ~BasicHashFunction();
};

class BKDRHashFunction : public HashFunction {
public:
    unsigned int seed;
    unsigned int dimission;
    unsigned int b;
public:
    unsigned int hash(char * data);
    unsigned int hash(int * data);
};


class HashFunctionFactory {
public:
    static HashFunction * createBasicHashFunction(unsigned int dimission, unsigned int b);
    static HashFunction * createBKDRHashFunction(unsigned int dimission, unsigned int b);
    static void initRandom();
    static int genRandomJiShuInt(int rangeStart, int rangeEnd);

private:
    static int genRandomInt(int rangeStart, int rangeEnd);

};
#endif //SRC_HASHFUNCTION_H
