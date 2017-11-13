#include <stdio.h>
#include <omp.h>
#include "matrix.h"
int main() {
    omp_set_num_threads(omp_get_max_threads());
    double* a[5];
    double ar1[] = {1,2,3,4};
    double ar2[] = {2,4,6,8};
    double ar3[] = {3,6,9,12};
    double ar4[] = {4,8,12,16};
    double ar5[] = {5,10,15,20}; 
    a[0] = &ar1[0];
    a[1] = &ar2[0];
    a[2] = &ar3[0];
    a[3] = &ar4[0];
    a[4] = &ar5[0];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%.2lf ", a[i][j]);
        }
        puts("");
    }
    double b[] = {1,2,3,4,5};
    double* opt = vector_alloc(4);
    right_multiply(5, 5, b, a, opt);
    for (int i = 0; i < 5; i++) {
        printf("%lf,", opt[i]);
        puts("");
    }
    return 0;
}