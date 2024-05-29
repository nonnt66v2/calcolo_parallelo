//
// Created by nonnt66 on 13/05/24.
//
//
// Created by nonnt66 on 04/04/24.
//
#include <stdio.h>
#include <omp.h>
#include <math.h>
#include "malloc.h"

#define N 8

double **allocaMat() {
    double **mat=(double**)malloc(N*sizeof(double*));
    for (int i = 0; i < N; ++i) {
        mat[i] = (double *) malloc(N * sizeof(double));
        for (int j = 0; j < N; ++j) {
            mat[i][j] = i+j;
        }
    }
    return mat;
}

//

int main() {
    double **mat = allocaMat();
    double *vet = (double *) malloc(N * sizeof(double));
    int p = 6;
    int i;
#pragma omp parallel for private(i) shared(mat, vet) num_threads(p)
    for (i = 0; i < N; i++) {
        vet[i] = mat[i][i];
    }
    i = 0;

    double maxValue = vet[0];
    double t0,t1;
    t0 = omp_get_wtime();
    if ((p == 2) || (p == 4) || (p == 8)) {
#pragma omp  parallel for private(i) shared(vet) reduction(max:maxValue) num_threads(p)
        for (i = 1; i < N; i++) {
            if (vet[i] > maxValue) {
                maxValue = vet[i];
            }
        }
    } else {
#pragma omp parallel for private(i) shared(vet, maxValue) num_threads(p)
        for (i = 1; i < N; i++) {
#pragma omp critical
            if (vet[i] > maxValue) {
                maxValue = vet[i];
            }
        }
    }
    t1 = omp_get_wtime();
    printf("massimo trovato in %.16lf secondi\n",t1-t0);
    printf("%.16lf\n", maxValue);

}