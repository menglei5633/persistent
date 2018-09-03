//
// Created by lm on 18-9-1.
//

#ifndef SRC_BITARRAY_H
#define SRC_BITARRAY_H
#define MASK 0x7
#define SHIFT 3

class BitArray {
public:
    void set_to_1(int location);
    int is_1(int location);
    void all_set_1();
    void all_set_0();
    BitArray(int bitSize);
    ~BitArray();
private:
    char * bits;
    int size;
};

#endif //SRC_BITARRAY_H
