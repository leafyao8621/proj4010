#include <stdio.h>
#include <omp.h>
#include "matrix.h"
#include "parser.h"

int main() {
    omp_set_num_threads(omp_get_max_threads());
    
    return 0;
}