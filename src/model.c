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

Model* new_Model(int num_non_basic,
                 int num_basic,
                 double* cn_vector,
                 double* xn_index_vector) {
    Model* opt = malloc(sizeof(Model));
    opt->num_non_basic = num_non_basic;
    opt->num_basic = num_basic;
    opt->cn_vector = cn_vector;
    opt->b_matrix = malloc(sizeof(double*) * num_basic);
    return opt;
}

int add_constraint(Model* model, double* row, int side, double val) {

}