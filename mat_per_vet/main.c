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
double **allocaMat(){
    double **mat = (double **)malloc(N*sizeof(double*));
    return mat;
}
//
double *matxvet(int m, int n, double *x, double **matPar) {
    int i, j;
    double *b = (double *) malloc(N * sizeof(double));
    int somma=0;
#pragma omp parallel for shared(m, n, matPar, x) private(i, j)  num_threads(8) reduction(+:somma)
    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            somma += matPar[i][j] * x[j];
        }
        b[i] = somma;
        somma=0;
    }
    return b;
}

int main() {
    double **mat = allocaMat();
    double *vet = (double *) malloc(N * sizeof(double));
    double t0, t1;
    for (int i = 0; i < N; ++i) {
        mat[i] = (double *) malloc(N * sizeof(double));
        for (int j = 0; j < N; ++j) {
            mat[i][j] = 1;
        }
    }
    for (int i = 0; i < N; ++i) {
        vet[i] = 1;
    }
    t0 = omp_get_wtime();
    double *vetFinale = matxvet(N, N, vet, *(&mat));
    t1 = omp_get_wtime();
    printf("il tempo di esecuzione è %.16lf\n", t1 - t0);
    for (int i = 0; i < N; ++i) {
        printf("%.16lf\n", vetFinale[i]);
    }

}