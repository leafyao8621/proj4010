#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "parser.h"
#include "matrix.h"

Model* model;

int init_model(char* file_name) {
    FILE* fin = fopen(file_name, "r");
    if (fin == NULL) {
        puts(file_name);
        puts("IO err");
        return 1;
    }
    int num_non_basic, num_basic;
    fscanf(fin, "%d %d", &num_non_basic, &num_basic);
    int is_max;
    fscanf(fin, "%d", &is_max);
    double* cn_vector = malloc(sizeof(double) * num_non_basic);
    double* xb_index_vector = malloc(sizeof(double) * num_basic);
    double* xn_index_vector = malloc(sizeof(double) * num_non_basic);
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int inc = omp_get_num_threads();
        for (int i = id; i < num_non_basic; i += inc) {
            xb_index_vector[i] = i + 1;
        }
    }
    for (int i = 0; i < num_basic; i++) {
        xn_index_vector[i] = i + num_non_basic + 1;
        fscanf(fin, "%lf", &cn_vector[i]);
    }
    if (!is_max) {
        vector_scalar_multiply(num_non_basic, cn_vector, -1, cn_vector);
    }
    model = new_Model(num_non_basic, num_basic, cn_vector, xn_index_vector, xb_index_vector);
    fclose(fin);
    return 0;
}

Model* get_model() {
    return model;
}
