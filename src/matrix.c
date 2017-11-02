#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "matrix.h"

double** matrix_alloc(int nr, int nc) {
    double** output = malloc(sizeof(double*) * nr);
    if (output == NULL) {
        puts("matrix alloc malloc error");
        return NULL;
    }
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int inc = omp_get_num_threads();
        for (int i = id; i < nr; i += inc) {
            output[i] = malloc(sizeof(double) * nc);
            if (output[i] == NULL) {
                puts("matrix alloc malloc error");
                exit(1);
            }
        }
    }
    return output;
}

int matrix_multiply(int nra, int nca, int ncb, double** a, double** b, double** output) {
    if (a == NULL)
    #pragma omp parallel
    {
    }
    return 0;
}