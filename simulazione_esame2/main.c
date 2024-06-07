#include <stdio.h>
#include <omp.h>
#include <math.h>
#include <time.h>
#include "stdlib.h"
#include "malloc.h"


void allocationArray(int** array, int N) {
    *array = (int*)calloc(N, sizeof(int));
}

void fillArray(int** array, int N) {
    for (int i = 0; i < N; i++)
        (*array)[i] = rand() % 11;
}

void printArray(int* array, int N, char* name) {
    printf("\nVector %s:\n", name);
    for (int i = 0; i < N; i++) {
        printf("%d    ", array[i]);
    }
    printf("\n");
}

void deallocaVet(int** array,int N) {
    free(*array);
}

void deallocaMat(int*** matrix, int N) {
    for (int i = 0; i < N; i++)
        free((*matrix)[i]);
    free(*matrix);
}

void initializeMatrix(int*** matrix, int N) {
    *matrix = (int**)calloc(N, sizeof(int*));
    for (size_t i = 0; i < N; ++i) {
        (*matrix)[i] = (int*)calloc(N, sizeof(int));
    }

    // Inizializzazione della matrice
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            (*matrix)[i][j] = (i + 1) * (j + 1);
        }
    }
}

void printMatrix(int** matrix, int N, char* name) {
    printf("\nMatrix %s:\n", name);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}
int main(int argc, char **argv) {
    if (argc < 3) {
        printf("fornire in input rispettivamente dimensione di matrice, scalari e num_threads\n");
        exit(-1);
    }
    int N = atoi(argv[1]);
//    int M = atoi(argv[2]);
    int numT = atoi(argv[2]);
//    int alpha = atoi(argv[4]);
//    int beta = atoi(argv[5]);
    int **matB = (int **) calloc(N, sizeof(int *));
    int **matA= (int **) calloc(N, sizeof(int *));

    int *vetA = (int *) calloc(N, sizeof(int));
    int *vetB = (int *) calloc(N, sizeof(int));

    double t0, t1, t2, t3;
    int nloc = N / numT;
    int i, j;
    int r;
    int step = 0;
    int *R = (int *) calloc(N, sizeof(int));
    int somma = 0;
    int indice = 0;
    t0 = omp_get_wtime();

#pragma omp master
    {
        for (int k = 0; k < N; k++) {
            matB[k] = (int *) calloc(N, sizeof(int));
            vetA[k] = 1;
            vetB[k] = 1;
            matA[k] = (int *) calloc(N, sizeof(int));
            for (int i=0;i<N;i++) {
                matB[k][i] = matA[k][i] = 1;
            }
        }
    };
    int k=0;
#pragma omp  parallel for private(k) shared(matA,vetB, N) num_threads(numT)
    for (k = 0; k < N; ++k) {
        matA[k][k] += vetB[k];
    }
    k=0;
    i=0;
    somma=0;
#pragma omp parallel for private(k,i) shared(matA,vetA,N) num_threads(numT) reduction(+:R[:N])
    for (k=0;k<N;k++){
        for(i=0;i<N;i++){
            R[i] += matA[i][k] * vetA[k];
        }
    }

    t1 = omp_get_wtime();
    //decommentare per stampa
//    for (int k = 0; k < N; k++) {
//        for (int i=0;i<N;i++) {
//            printf("%d ",matA[k][i]);
//        }
//        printf("\n");
//    }
//    for (int l = 0; l < N; ++l) {
//        printf("%d ",R[k]);
//
//    }
    deallocaMat(&matA,N);
    deallocaMat(&matB,N);
    deallocaVet(&vetA,N);
    deallocaVet(&vetB,N);

    printf("il calcolo ha richiesto [%.16lf] secondi\n", t1 - t0);
}
