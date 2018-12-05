#bin/bash
g++ -o  main.out -std=c++11 BloomFilter/BloomFilter.cpp BloomFilter/GeneralHashFunctions.cpp BloomFilter/HashFunction.cpp BloomFilter/BitArray.cpp HashTable/HashTable.cpp PresistentArgorithm/FindPresistent.cpp FileOption/FileOption.cpp main_muit.cpp > compile.result 

