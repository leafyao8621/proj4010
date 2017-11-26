#include <stdio.h>
#include <omp.h>
#include "matrix.h"
#include "parser.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        puts("Please pass in file name as cmd line argument");
        return 1;
    }
    omp_set_num_threads(omp_get_max_threads());
    init_model(argv[1]);
    solve(get_model());
    print_sol(get_model());
    return 0;
}