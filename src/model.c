#include "model.h"
#define LEQ 0
#define EQ 1
#define GEQ 2

struct Constraint {
    int side;
    double value;
};

struct Model {
    int num_non_basic;
    int num_basic;
    double** b_matrix;
    double** n_matrix;
    double* cb_vector;
    double* cn_vector;
    double* xb_index_vector;
    double* xn_index_vector;
    double* b_vector;
};
