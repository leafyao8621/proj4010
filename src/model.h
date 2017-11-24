typedef struct Model Model;
Model* new_Model(int num_non_basic,
                 int num_basic,
                 double* cn_vector,
                 int* xn_index_vector,
                 int* xb_index_vector);
int add_constraint(Model* model, int rn, double* row, int side, double val);
int pivot(Model* model, int in, int out);
int check(Model* model, int* ind);
int ratio_check(Model* model, int in_ind, int* ind);
int solve(Model* model);
int print_model(Model* model);