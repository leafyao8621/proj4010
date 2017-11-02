int multiply(int nra, int nca, int ncb, double** a, double** b, double** output);
int right_multiply(int da, int nrb, double* a, double** b, double** output);
int add(int nr, int nc, double** a, double** b);
int subtract(int nr, int nc, double** a, double** b);
int vector_add(int dim, double* a, double* b);
int vector_subtract(int dim, double* a, double* b);
int scalar_multiply(int nr, int nc, double** a, double factor);
int vector_scalar_multiply(int dim, double* a, double factor);
int inverse(int dim, double** a);
