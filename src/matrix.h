double** matrix_alloc(int nr, int nc);
int matrix_multiply(int nra, int nca, int ncb, double** a, double** b, double** output);
int right_multiply(int da, int ncb, double* a, double** b, double* output);
int left_multiply(int nra, int db, double** a, double* b, double* output);
int matrix_add(int nr, int nc, double** a, double** b, double** output);
int matrix_subtract(int nr, int nc, double** a, double** b, double** output);
int vector_add(int dim, double* a, double* b, double** output);
int vector_subtract(int dim, double* a, double* b, double** output);
int matrix_scalar_multiply(int nr, int nc, double** a, double factor, double** output);
int vector_scalar_multiply(int dim, double* a, double factor, double** output);
int matrix_invert(int dim, double** a, double** output);
int matrix_negate(int dim, double** a, double** output);
int vector_negate(int dim, double* a, double* output);
int free_matrix(int nr, double** a);
int swap_column(int ca, int cb, double** a, double** b);