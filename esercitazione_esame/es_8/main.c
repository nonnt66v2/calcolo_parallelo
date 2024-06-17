// Implementare un programma parallelo per l’ambiente
// multicore con np unità processanti che impieghi la
// libreria OpenMP. Il programma deve essere
// organizzato come segue: il core master deve generare
// una matrice A di dimensione N×M. Quindi, ogni core
// deve estrarre N/p colonne ed effettuare localmente lasomma degli elementi delle sottomatrici estratte,
// conservando il risultato in un vettore b di dimensione
// M.

#include <stdio.h>
#include <malloc.h>
#include <omp.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("inserire N & M & num_threads\n");
        exit(-1);
    }
    int N = atoi(argv[1]);
    int M = atoi(argv[2]);
    int numT = atoi(argv[3]);

    int **matA = (int **) calloc(N, sizeof(int *));
    int *vetC = (int *) calloc(M, sizeof(int));
    int i, j;
    for (i = 0; i < N; i++) {
        matA[i] = (int *) calloc(M, sizeof(int));
        for (j = 0; j < M; j++) {
            // matA[i][j] = rand()%100;
            matA[i][j] = 1;
        }
    }
    double t0, t1;
    t0 = omp_get_wtime();
    int colonneEstratte = N/numT;
#pragma omp parallel for private(i,j) shared(N,M,matA,colonneEstratte) num_threads(numT) reduction(+:vetC[:M])
    for (j = (omp_get_thread_num())*colonneEstratte; j < ((omp_get_thread_num())+1)*colonneEstratte; j++) {
        for (i = 0; i < M; i++) {
            vetC[i] += matA[i][j];
        }
    }

    t1 = omp_get_wtime();
    printf("caloclo eseguito in [%.16lf] secondi\n", t1 - t0);
    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            printf("%d\t", matA[i][j]);
        }
        printf("\n");
    }
    for (j = 0; j < M; j++) {
        printf("vet[%d] = %d \n", j, vetC[j]);
    }
}
