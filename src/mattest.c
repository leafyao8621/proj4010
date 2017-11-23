#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix.h"
#define NL puts("");
#define N 4000
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
    double** d = matrix_alloc(N, N);
    matrix_invert(N, b, d);
    // print_matrix(500, 500, b);
    // NL
    // print_matrix(500, 500, d);
    return 0;
}