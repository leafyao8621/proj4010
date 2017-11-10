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
    if (a == NULL || b == NULL || output == NULL) {
        puts("matrix multiply NULL ptr");
        return 1;
    }
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int inc = omp_get_num_threads();
        for (int i = id; i < nra; i += inc) {
            for (int j = 0; j < ncb; j++) {
                double temp = 0;
                for (int k = 0; k < nca; k++) {
                    temp += a[i][k] * b[k][j];
                }
                output[i][j] = temp;
            }
        }
    }
    return 0;
}

int matrix_add(int nr, int nc, double** a, double** b, double** output) {
    if (a == NULL || b == NULL || output == NULL) {
        puts("matrix add NULL ptr");
        return 1;
    }
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int inc = omp_get_num_threads();
        for (int i = id; i < nr; i += inc) {
            for (int j = 0; j < nc; j++) {
                output[i][j] = a[i][j] + b[i][j];
            }
        }
    }
    return 0;
}

int matrix_subtract(int nr, int nc, double** a, double** b, double** output) {
    if (a == NULL || b == NULL || output == NULL) {
        puts("matrix subtract NULL ptr");
        return 1;
    }
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int inc = omp_get_num_threads();
        for (int i = id; i < nr; i += inc) {
            for (int j = 0; j < nc; j++) {
                output[i][j] = a[i][j] - b[i][j];
            }
        }
    }
    return 0;
}

int right_multiply(int da, int ncb, double* a, double** b, double* output) {
    if (a == NULL || b == NULL || output == NULL) {
        puts("right multiply NULL ptr");
        return 1;
    }
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int inc = omp_get_num_threads();
        for (int i = id; i < ncb; i += inc) {
            double temp = 0;
            for (int j = 0; j < da; j++) {
                temp += a[j] * b[j][i];
            }
            output[i] = temp;
        }
    }
    return 0;
}

int left_multiply(int nra, int db, double** a, double* b, double* output) {
    if (a == NULL || b == NULL || output == NULL) {
        puts("right multiply NULL ptr");
        return 1;
    }
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int inc = omp_get_num_threads();
        for (int i = id; i < nra; i += inc) {
            double temp = 0;
            for (int j = 0; j < db; j++) {
                temp += a[i][j] * b[j];
            }
            output[i] = temp;
        }
    }
    return 0;
}