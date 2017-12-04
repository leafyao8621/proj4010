#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix.h"
#define NL puts("");
#define N 5
int main() {
    srand(time(NULL));
    double** a = matrix_alloc(N, N);
    double** b = matrix_alloc(N, N);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            a[i][j] = rand() % 10;
            b[i][j] = rand() % 10;
        }
    }
    
    //double** c = create_identity(5);
    // double** d = matrix_alloc(N, N);
    // matrix_invert(N, b, d);
    // print_matrix(500, 500, b);
    // NL
    // print_matrix(500, 500, d);
    double av[] = {3, 2, 1, 9, 8};
    double bv[] = {3, 1, 5, 4, 9};
    double* out = malloc(sizeof(double) * 5);
    double** c = matrix_alloc(5, 5);
    // print_matrix(5, 5, a);
    NL
    matrix_multiply(5, 5, 5, a, b, c);
    // print_matrix(5, 5, b);
    NL
    print_matrix(5, 5, c);
    NL
    matrix_realloc(5, 10, c);
    print_matrix(5, 10, c);
    return 0;
}