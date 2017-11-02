#include "model.h"
#define LEQ 0
#define EQ 1
#define GEQ 2

struct Variable {
    int index;
    double value;
    double objective_coef;
    double p1_coef;
};

struct Constraint {
    int side;
    Variable* slack;
    Variable* artificial;
};

struct Model {
    int num_var;
    double* non_basic_coef;
    double** basic_cons_mat;
    double** non_basic_cons_mat;
};