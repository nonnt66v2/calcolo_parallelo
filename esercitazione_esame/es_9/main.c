// Implementare un programma parallelo per l’ambiente
// multicore con np unità processanti che impieghi la
// libreria OpenMP. Il programma deve essere
// organizzato come segue: il core master deve generare
// due vettori casuali di dimensione N. Ogni core deve
// sommare ad ogni elemento dei due vettori il proprio
// identificativo e collaborare alla costruzione di una
// matrice, di dimensione N×M, con M=2np, le cui
// colonne, a due a due, sono i due vettori ottenuti nella
// fase precedente.

#include <stdio.h>
#include <malloc.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("inserire N & num_threads\n");
        exit(-1);
    }
    int N = atoi(argv[1]);

    int numT = atoi(argv[2]);
    int M = 2*numT;
    int **matA = (int **) calloc(N, sizeof(int *));
    int *vetC = (int *) calloc(N, sizeof(int));
    int *vetB = (int *) calloc(N, sizeof(int));
    int i, j;
    for (i = 0; i < N; i++) {
        matA[i] = (int *) calloc(M, sizeof(int));
        for (j = 0; j < M; j++) {
            // matA[i][j] = rand()%100;
            matA[i][j] = 0;
        }
    }
    srand(time(0));
    for(i=0;i<N;i++) {
        vetC[i] = rand()%10;
        vetB[i] = rand()%300;
    }
    double t0, t1;
    t0 = omp_get_wtime();
#pragma omp parallel for private (i) shared(N) num_threads(numT) reduction(+:vetC[:N]) reduction(+:vetB[:N])
    for(i=0;i<M;i++) {
        vetC[i] += omp_get_thread_num();
        vetB[i] += omp_get_thread_num();
    }
#pragma omp parallel for private(i,j) shared(N,M,matA,vetC,vetB) num_threads(numT)
    for (i = 0; i < M-1; i+=2) {
        for (j = 0; j < N; j++) {
            matA[j][i] = vetB[j];
            matA[j][i+1] = vetC[j];
        }
    }
    int massimo1 = vetB[0];
    int massimo2 = vetB[0];
#pragma omp parallel for private(i) shared(N) num_threads(numT) reduction(max:massimo1) reduction(max:massimo2)
    for (i=0;i<N;i++) {
        if(vetC[i] > massimo1) massimo1 = vetB[i];
        if(vetC[i] > massimo2) massimo2 = vetC[i];
    }

    t1 = omp_get_wtime();
    printf("il massimo di vetC [%d]\ned il massimo di vetB [%d]\nsono stati trovati in [%.16lf] secondi\n",massimo2,massimo1, t1 - t0);
    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            printf("%d\t", matA[i][j]);
        }
        printf("\n");
    }
    // for (j = 0; j < N; j++) {
    //     printf("vetC[%d] = %d\n", j, vetC[j]);
    //     printf("vetB[%d] = %d\n", j, vetB[j]);
    // }
}
