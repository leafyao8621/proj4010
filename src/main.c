#include <stdio.h>
#include <omp.h>
#include "matrix.h"
int main() {
    omp_set_num_threads(omp_get_max_threads());
    double** a;
    double** b;
    a = matrix_alloc(2, 2);
    b = matrix_alloc(2, 3);
    a[0][0] = 1;
    a[0][1] = 2;
    a[1][0] = 3;
    a[1][1] = 4;
    b[0][0] = 1;
    b[0][1] = 2;
    b[1][0] = 4;
    b[1][1] = 5;
    double** out;
    out = matrix_alloc(2, 2);
    matrix_multiply(2, 2, 2, a, b, out);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            printf("%lf,", out[i][j]);
        }
        puts("");
    }
    return 0;
}