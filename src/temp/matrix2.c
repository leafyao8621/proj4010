#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "../matrix.h"

int basic_matrix_multiply(int nralower, int nraupper, int nca, int ncblower, int ncbupper, double **a, double **b, double **out) {

    // TODO: Your code here. Don't forget to:
    // - check for invalid inputs!
    // - update return value
    if(a==NULL||b==NULL||out==NULL){
        printf("Matrix Null Pointer Exception");
        return 1;
    }
    for(int i=nralower;i<nraupper;i++){
        for(int k=ncblower;k<ncbupper;k++){
            out[i][k] = 0;
            for (int j=0;j<nca;j++){
                out[i][k] += a[i][j]*b[j][k];
            }
        }
    }
    return 0;
}

int matrix_multiply(int nra, int nca, int ncb, double** a, double** b, double** output) {
    if (a == NULL || b == NULL || output == NULL) {
        printf("Matrix Null Pointer Exception");
        return 1;
    }
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int nt = omp_get_num_threads();
        int split;
        if (nra>ncb) {
            split = nra;
        } else {
            split = ncb;
        }
        for (int i=id; i<split; i+=nt) {
            int uplimit;
            if ((i+nt)<=split) {
                uplimit = i+nt;
            } else {
                uplimit = split;
            }
            if (split==nra) {
                basic_matrix_multiply(i, uplimit, nca, 0, ncb, a, b, output);
            } else {
                basic_matrix_multiply(0, nra, nca, i, uplimit, a, b, output);
            }
        }
    }
    return 0;
}