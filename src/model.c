#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include "model.h"
#include "matrix.h"
#define LEQ 0
#define EQ 1
#define GEQ 2
#define UNF 0
#define OPT 1
#define INF 2
#define UBD 3
#define NL puts("");

struct Constraint {
    int side;
    double value;
};

struct Model {
    int stat;
    int num_non_basic;
    int num_basic;
    int num_art;
    double** b_matrix;
    double** n_matrix;
    double* cb_vector;
    double* cn_vector;
    int* xb_index_vector;
    int* xn_index_vector;
    double* b_vector;
    double** art_matrix;
    int* art_ind_vector;
};

Model* new_Model(int num_non_basic,
                 int num_basic,
                 double* cn_vector,
                 int* xn_index_vector,
                 int* xb_index_vector) {
    Model* opt = malloc(sizeof(Model));
    opt->stat = UNF;
    opt->num_non_basic = num_non_basic;
    opt->num_basic = num_basic;
    opt->cn_vector = cn_vector;
    opt->b_matrix = malloc(sizeof(double*) * num_basic);
    opt->n_matrix = malloc(sizeof(double*) * num_non_basic);
    opt->art_ind_vector = malloc(sizeof(double) * num_non_basic);
    opt->xn_index_vector = xn_index_vector;
    opt->xb_index_vector = xb_index_vector;
    opt->cb_vector = calloc(num_basic, sizeof(double));
    opt->art_matrix = NULL;
    opt->b_vector = malloc(sizeof(double) * num_basic);
    return opt;
}

int add_constraint(Model* model, int rn, double* row, int side, double val) {
    int negative = 0;
    if (side == GEQ) {
        if (val <= 0) {
            vector_scalar_multiply(model->num_non_basic, row, -1, row);
        } else {
            negative = 1;
            model->art_ind_vector[model->num_art++] = rn;
        }
    }
    if (side == LEQ && val < 0) {
        vector_scalar_multiply(model->num_non_basic, row, -1, row);
        negative = 1;
        model->art_ind_vector[model->num_art++] = -rn;
    }
    model->b_vector[rn] = val;
    model->n_matrix[rn] = row;
    double* b_vector = calloc(model->num_basic, sizeof(double));
    if (side != EQ) {
        b_vector[rn] = negative ? -1 : 1;
    } else {
        model->art_ind_vector[model->num_art++] = rn;
    }
    model->b_matrix[rn] = b_vector;
    return 0;
}

int check(Model* model, int* ind) {
    if (model == NULL || ind == NULL) {
        return -1;
    }
    double** temp = matrix_alloc(model->num_basic, model->num_non_basic);
    matrix_invert(model->num_basic, model->b_matrix, temp);
    matrix_multiply(model->num_basic, model->num_basic, model->num_non_basic, temp, model->n_matrix, temp);
    double* temp_v = malloc(sizeof(double) * model->num_non_basic);
    right_multiply(model->num_basic, model->num_non_basic, model->cb_vector, temp, temp_v);
    vector_subtract(model->num_non_basic, model->cn_vector, temp_v, temp_v);
    free_matrix(model->num_basic, temp);
    double max;
    find_max(model->num_non_basic, temp_v, ind, &max);
    return max >= 0;
}

int ratio_check(Model* model, int in_ind, int* ind) {
    if (model == NULL || ind == NULL) {
        puts("ratio check NULL ptr");
        return -1;
    }
    *ind = 0;
    double* temp = malloc(sizeof(double) * model->num_basic);
    extract_column(model->num_basic, in_ind, model->n_matrix, temp);
    double** b_inv = matrix_alloc(model->num_basic, model->num_basic);
    matrix_invert(model->num_basic, model->b_matrix, b_inv);
    left_multiply(model->num_basic, model->num_basic, b_inv, temp, temp);
    double min = model->b_vector[0] / temp[0];
    #pragma omp parallel for
    for (int i = 1; i < model->num_basic; i++) {
        if (model->b_vector[i] / temp[i] < min) {
            min = model->b_vector[i] /temp[i];
            *ind = i;
        }
    }
    return min >= 0;
}

int pivot(Model* model, int in, int out) {
    if (model == NULL) {
        puts("pivot NULL ptr");
        return 1;
    }
    int temp = model->xn_index_vector[in];
    model->xn_index_vector[in] = model->xb_index_vector[out];
    model->xb_index_vector[out] = temp;
    double temp1 = model->cn_vector[in];
    model->cn_vector[in] = model->cb_vector[out];
    model->cb_vector[out] = temp1;
    swap_column(in, out, model->num_basic, model->n_matrix, model->b_matrix);
    return 0;
}

int solve(Model* model) {
    int cond = 1;
    while (cond) {
        int in, out;
        int cond_check, cond_rchk;
        cond_check = check(model, &in);
        cond_rchk = ratio_check(model, in, &out);
        if (cond_check && cond_rchk) {
            pivot(model, in, out);
        } else {
            if (!cond_rchk) {
                model->stat = UBD;
            }
            cond = 0;
        }
        print_model(model);
    }
}

int print_model(Model* model) {
    printf("num_basic: %d\n", model->num_basic);
    printf("num_non_basic: %d\n\n", model->num_non_basic);
    puts("cb_vector:\n");
    print_vector(model->num_basic, model->cb_vector);
    NL
    puts("xb_index_vector:\n");
    print_vector_int(model->num_basic, model->xb_index_vector);
    NL
    puts("cn_vector:\n");
    print_vector(model->num_non_basic, model->cn_vector);
    NL
    puts("xn_index_vector:\n");
    print_vector_int(model->num_non_basic, model->xn_index_vector);
    NL
    puts("b_matrix:\n");
    print_matrix(model->num_basic, model->num_basic, model->b_matrix);
    NL
    puts("n matrix:\n");
    print_matrix(model->num_basic, model->num_non_basic, model->n_matrix);
    NL
}

int print_sol(Model* model) {
    if (model == NULL) {
        puts("print sol NULL ptr");
        return 1;
    }
    if (!model->stat) {
        puts("calculation in process");
        return 1;
    }
    double* xb = malloc(sizeof(double) * model->num_basic);
    left_multiply(model->num_basic, model->num_basic, model->b_matrix, model->b_vector, xb);
    for (int i = 0; i < model->num_basic; i++) {
        if (model->xb_index_vector[i] <= model->num_non_basic) {
            printf("x_%d = %.2lf\n\n", model->xb_index_vector[i], xb[i]);
        }
    }
}