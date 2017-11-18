#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix.h"
#define NL puts("");
int main() {
    srand(time(NULL));
    double** a = matrix_alloc(5, 5);
    double** b = matrix_alloc(5, 5);
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            a[i][j] = rand() % 10;
            b[i][j] = rand() % 10;
        }
    }
    
    double** c = create_identity(10);
    print_matrix(10, 10, c);
    return 0;
}