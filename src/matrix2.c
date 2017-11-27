#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "matrix.h"

double* vector_alloc(int n) {
    return malloc(sizeof(double) * n);
}

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

double** matrix_calloc(int nr, int nc) {
    double** output = malloc(sizeof(double*) * nr);
    if (output == NULL) {
        puts("matrix calloc malloc error");
        return NULL;
    }
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int inc = omp_get_num_threads();
        for (int i = id; i < nr; i += inc) {
            output[i] = calloc(nc, sizeof(double));
            if (output[i] == NULL) {
                puts("matrix calloc calloc error");
                exit(1);
            }
        }
    }
    return output;
}

int basic_matrix_multiply(int nralower, int nraupper, int nca, int ncblower, int ncbupper, double **a, double **b, double **out) {

    if(a==NULL||b==NULL||out==NULL){
        printf("Matrix Null Pointer Exception");
        return 1;
    }
    for(int i=nralower;i<nraupper;i++){
        for(int k=ncblower;k<ncbupper;k++){
            out[i][k] = 0;
            for (int j=0;j<nca;j++){
                out[i][k] += a[i][j]*b[j][k];
            }
        }
    }
    return 0;
}

int matrix_multiply(int nra, int nca, int ncb, double** a, double** b, double** output) {
    if (a == NULL || b == NULL || c == NULL) {
        printf("Matrix Null Pointer Exception");
        return 1;
    }
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int nt = omp_get_num_threads();
        if (nra>ncb) {
            int split = nra;
        } else {
            int split = ncb;
        }
        for (int i=id; i<split; i+=nt) {
            if ((i+nt)<=split) {
                int uplimit = i+nt;
            } else {
                int uplimit = split;
            }
            if (split==nra) {
                basic_matrix_multiply(i, uplimit, nca, 0, ncb, a, b, output);
            } else {
                basic_matrix_multiply(0, nra, nca, i, uplimit, a, b, output);
            }
        }
    }
    return 0;
}

int matrix_add(int nr, int nc, double** a, double** b, double** output) {
    if (a == NULL || b == NULL || output == NULL) {
        puts("cannot add any null matrices");
        return 1;
    }
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int nt = omp_get_num_threads();
        for (int i = id; i < nr; i += nt) {
            for (int j = 0; j < nc; j++) {
                output[i][j] = a[i][j] + b[i][j];
            }
        }
    }
    return 0;
}

int matrix_subtract(int nr, int nc, double** a, double** b, double** output) {
    if (a == NULL || b == NULL || output == NULL) {
        puts("cannot subtract any null matrices");
        return 1;
    }
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int nt = omp_get_num_threads();
        for (int i = id; i < nr; i += nt) {
            for (int j = 0; j < nc; j++) {
                output[i][j] = a[i][j] - b[i][j];
            }
        }
    }
    return 0;
}

//a vector multiplied by a matrix
int right_multiply(int da, int ncb, double* a, double** b, double* output) {
    if (a == NULL || b == NULL || output == NULL) {
        puts("cannot right multiply any null matrices");
        return 1;
    }
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int nt = omp_get_num_threads();
        for (int j = id; j < ncb; j += nt) {
            for (int i = 0; i < da; i++) {
                output[j] += a[i] * b[i][j];
            }
        }
    }
    return 0;
}

//a matrix multiplied by a vector
int left_multiply(int nra, int db, double** a, double* b, double* output) {
    if (a == NULL || b == NULL || output == NULL) {
        puts("cannot left multiply any null matrices");
        return 1;
    }
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int nt = omp_get_num_threads();
        for (int i = id; i < nra; i += nt) {
            for (int j = 0; j < db; j++) {
                output[i] += a[i][j] * b[j];
            }
        }
    }
    return 0;
}

int vector_add(int dim, double* a, double* b, double* output) {
    if (a == NULL || b == NULL || output == NULL) {
        puts("cannot add any null vectors");
        return 1;
    }
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int nt = omp_get_num_threads();
        for (int i = id; i < dim; i += nt) {
            output[i] = a[i] + b[i];
        }
    }
    return 0;
}

int vector_subtract(int dim, double* a, double* b, double* output) {
    if (a == NULL || b == NULL || output == NULL) {
        puts("cannot subtract any null vectors");
        return 1;
    }
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int nt = omp_get_num_threads();
        for (int i = id; i < dim; i += nt) {
            output[i] = a[i] - b[i];
        }
    }
    return 0;
}

int matrix_scalar_multiply(int nr, int nc, double** a, double factor, double** output) {
    if (a == NULL || output == NULL) {
        puts("cannot scalar multiply any null matrices");
        return 1;
    }
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int nt = omp_get_num_threads();
        for (int i = id; i < nr; i += nt) {
            for (int j = 0; j < nc; j++) {
                output[i][j] = a[i][j] * factor;
            }
        }
    }
    return 0;
}

int vector_scalar_multiply(int dim, double* a, double factor, double* output) {
    if (a == NULL || output == NULL) {
        puts("cannot scalar multiply any null vectors");
        return 1;
    }
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int nt = omp_get_num_threads();
        for (int i = id; i < dim; i += nt) {
            output[i] = a[i] * factor;
        }
    }
    return 0;
}

int free_matrix(int nr, double** a) {
    if (a == NULL) {
        puts("cannot free any null matrices");
        return 1;
    }
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int nt = omp_get_num_threads();
        for (int i = id; i < nr; i += nt) {
            free(a[i]);
        }
    }
    return 0;
}

int swap_column(int ca, int cb, int nr, double** a, double** b) {
    if (a == NULL || b == NULL) {
        puts("cannot swap column in any null matrices");
        return 1;
    }
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int nt = omp_get_num_threads();
        for (int i = id; i < nr; i += nt) {
            double temp = a[i][ca];
            a[i][ca] = b[i][cb];
            b[i][cb] = temp;
        }
    }
    return 0;
}

int swap_row(int ra, int rb, double** a, double** b) {
    if (a == NULL || b == NULL) {
        puts("cannot swap row in any null matrices" );
        return 1;
    }
    double* temp = a[ra];
    a[ra] = b[rb];
    b[rb] = temp;
    return 0;
}

int print_vector(int dim, double* vector) {
    if (vector == NULL) {
        puts("cannot print any null vectors");
        return 1;
    }
    for (int i = 0; i < dim - 1; i++) {
        printf("%.2lf ", vector[i]);
    }
    printf("%.2lf\n", vector[dim - 1]);
    return 0;
}

int print_matrix(int nr, int nc, double** matrix) {
    if (matrix == NULL) {
        puts("cannot print any null matrices");
        return 1;
    }
    for (int i = 0; i < nr; i++) {
        for (int j = 0; j < nc - 1; j++) {
            printf("%.2lf ", matrix[i][j]);
        }
        printf("%.2lf\n", matrix[i][nc - 1]);
    }
    return 0;
}

int print_vector_int(int dim, int* vector) {
    if (vector == NULL) {
        puts("cannot print any null vectors");
        return 1;
    }
    for (int i = 0; i < dim - 1; i++) {
        printf("%d ", vector[i]);
    }
    printf("%d\n", vector[dim - 1]);
    return 0;
}

int print_matrix_int(int nr, int nc, int** matrix) {
    if (matrix == NULL) {
        puts("cannot print any null matrices");
        return 1;
    }
    for (int i = 0; i < nr; i++) {
        for (int j = 0; j < nc - 1; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("%d\n", matrix[i][nc - 1]);
    }
    return 0;
}

int matrix_copy(int nr, int nc, double** from, double** to) {
    if (from == NULL || to == NULL) {
        puts("cannot copy any null matrices");
        return 1;
    }
    #pragma omp parallel for
    for (int i = 0; i < nr; i++) {
        memcpy(to[i], from[i], sizeof(double) * nc);
    }
    return 0;
}

double** create_identity(int dim) {
    double** opt = malloc(dim * sizeof(double*));
    if (opt == NULL) {
        puts("cannot create identity malloc");
        return NULL;
    }
    #pragma omp parallel for
    for (int i = 0; i < dim; i++) {
        opt[i] = calloc(dim, sizeof(double));
        if (opt[i] == NULL) {
            puts("cannot create create identity calloc");
            exit(1);
        }
        opt[i][i] = 1;
    }
    return opt;
}
