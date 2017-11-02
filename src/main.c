#include <stdio.h>
#include <omp.h>
#include "matrix.h"
int main() {
    omp_set_num_threads(omp_get_max_threads());
    double** a;
    a = matrix_alloc(2, 2);
    a[0][0] = 1;
    a[0][1] = 2;
    a[1][0] = 3;
    a[1][1] = 4;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            printf("%lf,", a[i][j]);
        }
        puts("");
    }
    return 0;
}