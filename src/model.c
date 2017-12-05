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

struct Solution {
    int stat;
    int num_var;
    double val;
    int* ind;
    double* sol;
};

struct Model {
    int is_max;
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
    int* art_ind_vector;
};

Model* new_Model(int is_max,
                 int num_non_basic,
                 int num_basic,
                 double* cn_vector,
                 int* xn_index_vector,
                 int* xb_index_vector) {
    Model* opt = malloc(sizeof(Model));
    opt->is_max = is_max;
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

int ratio_check(Model* model, int in_ind, int* ind) {
    if (model == NULL || ind == NULL) {
        puts("ratio check NULL ptr");
        return -1;
    }
    double* temp = malloc(sizeof(double) * model->num_basic);
    extract_column(model->num_basic, in_ind, model->n_matrix, temp);
    double** b_inv = matrix_alloc(model->num_basic, model->num_basic);
    matrix_invert(model->num_basic, model->b_matrix, b_inv);
    left_multiply(model->num_basic, model->num_basic, b_inv, temp, temp);
    double* temp1 = malloc(sizeof(double) * model->num_basic);
    left_multiply(model->num_basic, model->num_basic, b_inv, model->b_vector, temp1);
    int min_set = 0;
    double min;
    *ind = -1;
    for (int i = 0; i < model->num_basic; i++) {
        if (!min_set) {
            if (temp1[i] / temp[i] >= 0) {
                *ind = i;
                min = temp1[i] / temp[i];
                min_set = 1;
            }
        }
        if (temp1[i] / temp[i] < min && temp1[i] / temp[i] >= 0) {
            min = temp1[i] / temp[i];
            *ind = i;
        }
    }
    free(temp);
    free(temp1);
    free_matrix(model->num_basic, b_inv);
    return min_set;
}

int check(Model* model, int* in, int* out) {
    if (model == NULL || in == NULL || out == NULL) {
        return -1;
    }
    double** b_inv = matrix_alloc(model->num_basic, model->num_basic);
    double** temp = matrix_alloc(model->num_basic, model->num_non_basic);
    matrix_multiply(model->num_basic, model->num_basic, model->num_non_basic, b_inv, model->n_matrix, temp);
    free_matrix(model->num_basic, b_inv);
    double* temp_v = malloc(sizeof(double) * model->num_non_basic);
    right_multiply(model->num_basic, model->num_non_basic, model->cb_vector, temp, temp_v);
    free_matrix(model->num_basic, temp);
    vector_subtract(model->num_non_basic, model->cn_vector, temp_v, temp_v);
    
    double max;
    find_max(model->num_non_basic, temp_v, in, &max); 
    if (max <= 0) {
        model->stat = OPT;
        free(temp_v);
        return 0;
    }
    find_first_pos(model->num_non_basic, temp_v, in);
    int max_ind = *in;
    while (*in != -1) {
        int cond = ratio_check(model, *in, out);
        if (!cond) {
            model->stat = UBD;
        } else {
            if (temp_v[*in] > temp_v[max_ind]) {
                max_ind = *in;
            }
            
        }
        find_next_pos(model->num_non_basic, *in, temp_v, in);
    }
    *in = max_ind;
    free(temp_v);
    return ratio_check(model, *in, out);
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
    if (model == NULL) {
        puts("solve NULL ptr");
        return 1;
    }
    int cond = 1;
    while (cond) {
        int in, out;
        cond = check(model, &in, &out);
        if (cond) {
            pivot(model, in, out);
        }
    }
    return 0;
}

int remove_art(Model* model, double* backup) {
    if (model == NULL || backup == NULL) {
        puts("remove art NULL ptr");
        return 1;
    }
    free(model->cb_vector);
    model->cb_vector = calloc(model->num_basic, sizeof(double));
    free(model->cn_vector);
    model->cn_vector = malloc(sizeof(double) * (model->num_non_basic - model->num_art));
    int* t_v = malloc(sizeof(double) * (model->num_non_basic - model->num_art));
    double** t_m = matrix_alloc(model->num_basic, model->num_non_basic - model->num_art);
    int ind = 0;
    for (int i = 0; i < model->num_non_basic; i++) {
        if (model->xn_index_vector[i] <= model->num_non_basic) {
            model->cn_vector[ind] = backup[model->xn_index_vector[i] - 1];
            t_v[ind] = model->xn_index_vector[i];
            copy_column(model->num_basic, i, ind++, model->n_matrix, t_m);            
        } else if (model->xn_index_vector[i] <= model->num_non_basic - model->num_art + model->num_basic) {
            model->cn_vector[ind] = 0;
            t_v[ind] = model->xn_index_vector[i];
            copy_column(model->num_basic, i, ind, model->n_matrix, t_m);
        }
    }
    for (int i = 0; i < model->num_basic; i++) {
        if (model->xb_index_vector[i] <= model->num_non_basic) {
            model->cb_vector[i] = backup[model->xb_index_vector[i] - 1];
        }
    }
    free(model->xn_index_vector);
    model->xn_index_vector = t_v;
    free_matrix(model->num_basic, model->n_matrix);
    model->n_matrix = t_m;
    model->num_non_basic -= model->num_art;
    return 0;
}

int phase_one(Model* model) {
    if (model == NULL) {
        puts("phase one NULL ptr");
        return 1;
    }
    if (!model->num_art) {
        solve(model);
        return 0;
    }
    // print_model(model);
    double* backup = malloc(sizeof(double) * model->num_non_basic);
    memcpy(backup, model->cn_vector, sizeof(double) * model->num_non_basic);
    free(model->cn_vector);
    model->cn_vector = calloc(model->num_non_basic + model->num_art, sizeof(double));
    model->xn_index_vector = realloc(model->xn_index_vector,
                                     sizeof(int) * (model->num_non_basic +
                                     model->num_art));
    matrix_realloc(model->num_basic, model->num_non_basic + model->num_art,
                   model->n_matrix);
    int ind = 0;
    for (int i = 0; i < model->num_basic; i++) {
        if (i == model->art_ind_vector[ind]) {
            model->cb_vector[i] = -1;
            model->xb_index_vector[i] = model->num_non_basic +
                                        model->num_basic + ind + 1;
            model->xn_index_vector[ind + model->num_non_basic] = model->num_non_basic + i + 1;
            model->n_matrix[model->art_ind_vector[ind]][ind + model->num_non_basic] = model->b_matrix[model->art_ind_vector[ind]][model->art_ind_vector[ind]];
            model->b_matrix[model->art_ind_vector[ind]][model->art_ind_vector[ind]] *= -1;
            if (!model->b_matrix[model->art_ind_vector[ind]][model->art_ind_vector[ind]]) {
                model->b_matrix[model->art_ind_vector[ind]][model->art_ind_vector[ind]] = 1;
            }
            ind++;
        } else {
            model->cb_vector[i] = 0;
            model->xb_index_vector[i] = model->num_non_basic + i + 1;
        }
    }
    model->num_non_basic += model->num_art;
    free(model->art_ind_vector);
    model->art_ind_vector = NULL;
    print_model(model);
    solve(model);
    // print_model(model);
    print_sol(model);
    Solution* s = get_sol(model);
    // printf("%d\n", s->stat == UBD || s->val != 0);
    if (s->stat == UBD || s->val != 0) {
        model->stat = INF;
        // printf("%d %lf\n", s->stat == UBD, s->val);
        free_Solution(s);
        return 0;
    }
    // puts("phase2");
    
    // print_model(model);
    remove_art(model, backup);
    // print_model(model);
    solve(model);
    free_Solution(s);
    return 0;
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
    puts("n_matrix:\n");
    print_matrix(model->num_basic, model->num_non_basic, model->n_matrix);
    NL
    puts("b_vector:\n");
    print_vector(model->num_basic, model->b_vector);
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
    switch (model->stat) {
    case OPT:
        break;
    case INF:
        puts("Infeasible");
        return 0;
    case UBD:
        puts("Unbounded");
        return 0;
    default:
        puts("status error");
        return 1;
    }
    double* xb = malloc(sizeof(double) * model->num_basic);
    double** b_inv = matrix_alloc(model->num_basic, model->num_basic);
    matrix_invert(model->num_basic, model->b_matrix, b_inv);
    left_multiply(model->num_basic, model->num_basic, b_inv, model->b_vector, xb);
    for (int i = 0; i < model->num_basic; i++) {
        if (model->xb_index_vector[i] <= model->num_non_basic) {
            printf("x_%d = %lf\n\n", model->xb_index_vector[i], xb[i]);
        }
    }
    for (int i = 0; i < model->num_non_basic; i++) {
        if (model->xn_index_vector[i] <= model->num_non_basic) {
            printf("x_%d = %lf\n\n", model->xn_index_vector[i], 0.0);
        }
    }
    double opt_val;
    dot_product(model->num_basic, model->cb_vector, xb, &opt_val);
    printf("opt_val = %lf\n", model->is_max ? opt_val : -opt_val);
    free(xb);
    free_matrix(model->num_basic, b_inv);
    return 0;
}

Solution* get_sol(Model* model) {
    if (model == NULL) {
        puts("get opt NULL ptr");
        return NULL;
    }
    Solution* s = malloc(sizeof(Solution));
    s->stat = model->stat;
    s->num_var = model->num_non_basic;
    s->ind = malloc(sizeof(int) * s->num_var);
    s->sol = malloc(sizeof(double) * s->num_var);
    double* xb = malloc(sizeof(double) * model->num_basic);
    double** b_inv = matrix_alloc(model->num_basic, model->num_basic);
    matrix_invert(model->num_basic, model->b_matrix, b_inv);
    left_multiply(model->num_basic, model->num_basic, b_inv, model->b_vector, xb);
    dot_product(model->num_basic, model->cb_vector, xb, &(s->val));
    for (int i = 0; i < model->num_basic; i++) {
        if (model->xb_index_vector[i] <= model->num_non_basic) {
            s->ind[i] = model->xb_index_vector[i];
            s->sol[i] = xb[i];
        }
    }
    for (int i = 0; i < model->num_non_basic; i++) {
        if (model->xn_index_vector[i] <= model->num_non_basic) {
            s->ind[i] = model->xn_index_vector[i];
            s->sol[i] = 0.0;
        }
    }
    free(xb);
    free_matrix(model->num_basic, b_inv);
    return s;
}

int free_Solution(Solution* s) {
    if (s == NULL) {
        puts("free solution NULL ptr");
        return 1;
    }
    free(s->ind);
    free(s->sol);
    free(s);
    return 0;
}

int free_Model(Model* model) {
    if (model == NULL) {
        puts("free model NULL ptr");
        return 1;
    }
    free(model->cb_vector);
    free(model->cn_vector);
    free(model->b_vector);
    free(model->xb_index_vector);
    free(model->xn_index_vector);
    free(model->art_ind_vector);
    free_matrix(model->num_basic, model->b_matrix);
    free_matrix(model->num_basic, model->n_matrix);
    free(model);
    return 0;
}