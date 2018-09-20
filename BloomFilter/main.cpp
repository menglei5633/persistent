//
// Created by lm on 18-9-1.
//

#include <stdio.h>
#include "BitArray.h"
#include "HashFunction.h"
#include "BloomFilter.h"

int main() {
    HashFunctionFactory::initRandom();
    BloomFilter bl(8, 2, 100);
    int a[] = {1, 3, 4, 5, 6, 7, 8, 10};
    char * b = "vhbajkf";
    char * c = "gfkad";
    bl.add(a);
    bl.add(c);
    if (bl.is_inserted(a)) {
        printf("a\n");
    }
    if (bl.is_inserted(b)) {
        printf("b\n");
    }
    if (bl.is_inserted(c)) {
        printf("c\n");
    }
    bl.all_set_1();
    printf("after\n");
    if (bl.is_inserted(a)) {
        printf("a\n");
    }
    if (bl.is_inserted(b)) {
        printf("b\n");
    }
    if (bl.is_inserted(c)) {
        printf("c\n");
    }
    return 0;
}
