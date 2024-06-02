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

    double t0, t1, t2, t3;
    int nloc = N / numT;
    int i, j;
    int r;
    int step = 0;
    int *R = (int *) calloc(M, sizeof(int));
    int somma = 0;
    int indice = 0;
    t0 = omp_get_wtime();
    for (int k = 0; k < N; k++) {
        mat[k] = (int *) calloc(M, sizeof(int));
    }
#pragma omp parallel private(i, j, step, indice) shared(mat, vetA, alpha, beta, nloc, N, M, R) num_threads(numT)
    {

        r = N % numT;
        nloc = N / numT;
        if (omp_get_thread_num() < r) {
            nloc++;
            step = 0;
        } else {
            step = r;
        }
        t2 = omp_get_wtime();
        for (i = 0; i < nloc; i++) {
            indice = i + nloc * omp_get_thread_num() + step;
//        vetA[i] = (int) rand();
            vetA[indice] = 1;
        }
        for (i = 0; i < nloc; i++) {
            indice = i + nloc * omp_get_thread_num() + step;
            vetB[indice] = 1;
        }
        for (i = 0; i < nloc; i++) {
            indice = i + nloc * omp_get_thread_num() + step;
            for (j = 0; j < M; j++) {
//            mat[i][j] = (int) rand();
                mat[indice][j] = 1;
            }
        }
        t3 = omp_get_wtime();
        printf("indice [%d] thread [%d] -> %.16lf\n", indice, omp_get_thread_num(), t3 - t2);
        indice = 0;
        for (i = 0; i < nloc; i++) {
            for (j = 0; j < M; j++) {
                indice = i + nloc * omp_get_thread_num() + step;
                vetA[indice] *= mat[indice][j];
                vetB[indice] *= mat[j][indice];
            }
        }
        for (i = 0; i < nloc; i++) {
            for (j = 0; j < M; j++) {
                indice = i + nloc * omp_get_thread_num() + step;
                R[indice] = alpha * mat[indice][j] * beta + vetB[indice] * alpha;
            }
        }

    }
    t1 = omp_get_wtime();
    //decommentare per stampa
    for (int k = 0; k < M; ++k) {
        printf("vet[%d] = %d\n", k, R[k]);
    }
    printf("il calcolo ha richiesto [%.16lf] secondi\n", t1 - t0);
}
