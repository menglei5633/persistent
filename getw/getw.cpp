//
// Created by lm on 18-9-12.
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX 10000


int main(int argc, char* argv[]) {
    if (argc < 7) {
        return -1;
    }
    int T = atoi(argv[1]);
    int N = atoi(argv[2]);
    int Np = atoi(argv[3]);
    int r = atoi(argv[4]);
    int S = atoi(argv[5]);
    float Pfp = atof(argv[6]);

    float max_pfp;
    int i;
    int w = 1;
    float min_value = MAX;
    float current_value = 0;
    float et = 0;
    float max_fnp = 0;
    int k;

    for (k = 2; k < 30; ++k) {
        for (w = 1; w < T; ++w) {
            et = 1.0 * (pow(2, -1 * k * (T - w - S)) * N + Np) / w;
            current_value = et + r * pow(2, -1 * k * (T - w - S));
//            printf("%f\n", current_value);
//            if (current_value > min_value) {
//                break;
//            } else {
//                min_value = current_value;
//            }
            max_fnp = (log(log(et)) + log(1 + 0.01)) / pow(et, 0.01);
            max_pfp = pow(2, -1 * k * (T - w - S));
//            printf("%f \n", fabs(max_pfp - Pfp));
            if (max_fnp >= 0.01 && max_fnp <= 0.1) {
                printf("k : %d     w : %d    Pfp : %f     Pnp : %f     min_w : %f\n", k, w, max_pfp, max_fnp, current_value);
//                printf("1\n");
            }
        }


    }

    return 0;
}