typedef struct Exc Exc;
double* vector_alloc(int n);
double** matrix_alloc(int nr, int nc);
int matrix_realloc(int nr, int nc, double** a);
int matrix_multiply(int nra, int nca, int ncb, double** a, double** b, double** output);
int right_multiply(int da, int ncb, double* a, double** b, double* output);
int left_multiply(int nra, int db, double** a, double* b, double* output);
int matrix_add(int nr, int nc, double** a, double** b, double** output);
int matrix_subtract(int nr, int nc, double** a, double** b, double** output);
int vector_add(int dim, double* a, double* b, double* output);
int vector_subtract(int dim, double* a, double* b, double* output);
int matrix_scalar_multiply(int nr, int nc, double** a, double factor, double** output);
int vector_scalar_multiply(int dim, double* a, double factor, double* output);
int matrix_invert(int dim, double** a, double** output);
int free_matrix(int nr, double** a);
int swap_column(int ca, int cb, int nr, double** a, double** b);
int swap_row(int ra, int rb, double** a, double** b);
int print_vector(int dim, double* vector);
int print_matrix(int nr, int nc, double** matrix);
int print_vector_int(int dim, int* vector);
int print_matrix_int(int nr, int nc, int** matrix);
int matrix_copy(int nr, int nc, double** from, double** to);
int find_max(int dim, double* vector, int* max_ind, double* max);
int extract_column(int nr, int cn, double** matrix, double* output);
int copy_column(int nr, int acn, int bcn, double** a, double** b);
double** create_identity(int dim);
int dot_product(int dim, double* a, double* b, double* output);
int find_first_pos(int dim, double* vector, int* ind);
int find_next_pos(int dim, int start, double* vector, int* ind);