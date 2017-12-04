typedef struct Model Model;
typedef struct Solution Solution;

Model* new_Model(int is_max,
                 int num_non_basic,
                 int num_basic,
                 double* cn_vector,
                 int* xn_index_vector,
                 int* xb_index_vector);
int add_constraint(Model* model, int rn, double* row, int side, double val);
int print_model(Model* model);
int print_sol(Model* model);
int phase_one(Model* model);
int free_Model(Model* model);
int free_Solution(Solution* s);
Solution* get_sol(Model* model);