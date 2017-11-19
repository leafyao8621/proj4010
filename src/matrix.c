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

int matrix_multiply(int nra, int nca, int ncb, double** a, double** b, double** output) {
    if (a == NULL || b == NULL || output == NULL) {
        puts("matrix multiply NULL ptr");
        return 1;
    }
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < nra; i++) {
        for (int j = 0; j < ncb; j++) {
            double temp = 0;
            #pragma omp parallel for
            for (int k = 0; k < nca; k++) {
                temp += a[i][k] * b[k][j];
            }
            output[i][j] = temp;
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

int vector_add(int dim, double* a, double* b, double* output) {
    if (a == NULL || b == NULL || output == NULL) {
        puts("vector add NULL ptr");
        return 1;
    }
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int inc = omp_get_num_threads();
        for (int i = id; i < dim; i += inc) {
            output[i] = a[i] + b[i];
        }
    }
    return 0;
}

int vector_subtract(int dim, double* a, double* b, double* output) {
    if (a == NULL || b == NULL || output == NULL) {
        puts("vector subtract NULL ptr");
        return 1;
    }
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int inc = omp_get_num_threads();
        for (int i = id; i < dim; i += inc) {
            output[i] = a[i] - b[i];
        }
    }
    return 0;
}

int matrix_scalar_multiply(int nr, int nc, double** a, double factor, double** output) {
    if (a == NULL || output == NULL) {
        puts("matrix scalar multiply NULL ptr");
        return 1;
    }
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int inc = omp_get_num_threads();
        for (int i = id; i < nr; i += inc) {
            for (int j = 0; j < nc; j++) {
                output[i][j] = a[i][j] * factor;
            }
        }
    }
    return 0;
}

int vector_scalar_multiply(int dim, double* a, double factor, double* output) {
    if (a == NULL || output == NULL) {
        puts("vector scalar multiply NULL ptr");
        return 1;
    }
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int inc = omp_get_num_threads();
        for (int i = id; i < dim; i += inc) {
            output[i] = a[i] * factor;
        }
    }
    return 0;
}

int free_matrix(int nr, double** a) {
    if (a == NULL) {
        puts("free matrix NULL ptr");
        return 1;
    }
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int inc = omp_get_num_threads();
        for (int i = id; i < nr; i += inc) {
            free(a[i]);
        }
    }
    return 0;
}

int swap_column(int ca, int cb, int nr, double** a, double** b) {
    if (a == NULL || b == NULL) {
        puts("swap column NULL ptr");
        return 1;
    }
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int inc = omp_get_num_threads();
        for (int i = id; i < nr; i += inc) {
            double temp = a[i][ca];
            a[i][ca] = b[i][cb];
            b[i][cb] = temp;
        }
    }
    return 0;
}

int swap_row(int ra, int rb, double** a, double** b) {
    if (a == NULL || b == NULL) {
        puts("swap row NULL ptr");
        return 1;
    }
    double* temp = a[ra];
    a[ra] = b[rb];
    b[rb] = temp;
    return 0;
}

int print_vector(int dim, double* vector) {
    if (vector == NULL) {
        puts("print vector NULL ptr");
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
        puts("print matrix NULL ptr");
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
        puts("print vector NULL ptr");
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
        puts("print matrix NULL ptr");
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
        puts("matrix copy NULL ptr");
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
        puts("create identity malloc err");
        return NULL;
    }
    #pragma omp parallel for
    for (int i = 0; i < dim; i++) {
        opt[i] = calloc(dim, sizeof(double));
        if (opt[i] == NULL) {
            puts("create identity calloc err");
            exit(1);
        }
        opt[i][i] = 1;
    }
    return opt;
}

int matrix_invert(int dim, double** a, double** output) {
    if (a == NULL || output == NULL) {
        puts("matrix invers NULL ptr");
        return 1;
    }
    matrix_copy(dim, dim, a, output);
    
    double** i_matrix = create_identity(dim);
    for (int i = 0; i < dim; i++) {
        if (output[i][i] == 0) {
            int j;
            for (j = i; j < dim && output[i][j] == 0; j++);
            if (output[i][j] == 0) {
                return -1;
            }
            swap_row(i, j, output, output);
        }
        double scale = output[i][i];
        #pragma omp parallel for
        for (int j = 0; j < dim; j++) {
            output[i][j] = output[i][j] / scale;
            i_matrix[i][j] = i_matrix[i][j] / scale;
        }
        if (i < dim - 1) {
            #pragma omp parallel for
            for (int j = i + 1; j < dim; j++) {
                double factor = output[j][i];
                #pragma omp parallel for
                for (int k = 0; k < dim; k++) {
                    output[j][k] = output[j][k] - factor * output[i][k];
                    i_matrix[j][k] = i_matrix[j][k] - factor * i_matrix[i][k];
                }
            }
        }
    }
    for (int i = dim - 1; i > 0; i--) {
        #pragma parallel for
        for (int j = i - 1; j > -1; j--) {
            double factor = output[j][i];
            #pragma omp parallel for
            for (int k = 1; k < dim; k++) {
                output[j][k] = output[j][k] - factor * output[i][k];
                i_matrix[j][k] = i_matrix[j][k] - factor * i_matrix[i][k];
            }
        }
    }
    matrix_copy(dim, dim, i_matrix, output);
    free_matrix(dim, i_matrix);
    return 0;
}