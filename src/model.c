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
    int num_non_basic;
    int num_basic;
};