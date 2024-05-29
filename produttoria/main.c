#include <stdio.h>
#include <omp.h>
#include <math.h>
#include <time.h>
#include "stdlib.h"
#include "malloc.h"

int main(int argc, char **argv) {
    if (argc < 6) {
        printf("fornire in input rispettivamente dimensione di matrice, scalari e num_threads\n");
        exit(-1);
    }
    int N = atoi(argv[1]);
    int M = atoi(argv[2]);
    int numT = atoi(argv[3]);
    int alpha = atoi(argv[4]);
    int beta = atoi(argv[5]);
    int **mat = (int **) calloc(N, sizeof(int *));
    int *vetA = (int *) calloc(M, sizeof(int));
    int *vetB = (int *) calloc(N, sizeof(int));
    for (int i = 0; i < N; i++) {
        mat[i] = (int *) calloc(M, sizeof(int));
        for (int j = 0; j < M; j++) {
//            mat[i][j] = (int) rand();
            mat[i][j] = 2;

        }
    }
    for (int i = 0; i < M; i++) {
//        vetA[i] = (int) rand();
        vetA[i] = 2;
    }
    for (int i = 0; i < N; i++) {
        vetB[i] = (int) rand();
    }
    int nloc = N / numT;
    int i, j;
    int r;
    int step = 0;
    int *R = (int *) calloc(M, sizeof(int));
    int somma = 0;
#pragma omp parallel private(i, j, step) shared(mat, vetA, alpha, beta, nloc, N, M, R) num_threads(numT) reduction(+:somma)
    {

        r = N % numT;
        nloc = N / numT;
        if (omp_get_thread_num() < r) {
            nloc++;
            step = 0;
        } else {
            step = r;
        }
        int id = omp_get_thread_num();
        for (i = id * (N / numT); i < ((id + 1) * (N / numT)); i++) {
            for (j = 0; j < M; j++) {
                vetA[i] *= mat[i][j];
                vetB[i] *= mat[j][i];
            }
        }
        for (i = id * (N / numT); i < ((id + 1) * (N / numT)); i++) {
            R[i] = alpha * vetA[i] * beta + vetB[i] * alpha;
        }

    }
    for (int k = 0; k < M; ++k) {
        printf("vet[%d] = %d\n", k, vetA[k]);
    }


}
