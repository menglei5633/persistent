#include <stdio.h>
#include <assert.h>



class RandomGenerate {
private:
    int start;
    int end;
    int length;
    bool* card;
    int returnNum(int r) {
        int i, j;
        j = -1;
        for (i = 0; i < r; ++i) {
            ++j;
            while (this->card[j] == 1) {
                ++j;
            }
        }
        return j;
    }
    int genRandomInt(int rangeStart, int rangeEnd) {
        assert(rangeStart <= rangeEnd);
        int r;
        r = rangeStart + (int)((rangeEnd - rangeStart + 1.0) * random()/ (RAND_MAX + 1.0));
        assert(r >= rangeStart && r <= rangeEnd);
        return r;
    }
public:
    RandomGenerate(int s, int e) {
        this->start = s;
        this->end = e;
        this->length = e - s;
        this->card = (bool*)malloc((this->length) * sizeof(bool));
        for (int j = 0; j < this->length; j++) {
            this->card[j] = 0;
        }
    }

    void printfCard() {
        for (int i=0; i < this->length; i++) {
            printf("%d ", card[i]);
        }
        printf("\n");
    }

    int getRandom() {
        int r = genRandomInt(0, this->length-1);
        // printf("__:%d\n", r);
        int j = this->returnNum(r+1);
        // printf("j:%d\n", j);
        this->card[j] = 1;
        this->length -- ;
        return this->start + j;
    }

    void init() {
        this->length = this->end - this->start;
        for (int j = 0; j < this->length; j++) {
            this->card[j] = 0;
        }
    }

    bool isEnd() {
        return this->length == 0;
    }


    ~RandomGenerate() {
        free(this->card);
    }
};
