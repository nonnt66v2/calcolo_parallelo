/**Implementare un programma parallelo per
l’ambiente multicore con np×np unità processanti che
impieghi la libreria OpenMP. Il programma deve essere
organizzato come segue: il core master deve generare
una matrice A di dimensione N×N. Quindi, ogni core
deve sommare tra loro gli elementi di un bloccoquadrato della matrice A di dimensione (N/np)×(N/np),
conservando i valori in un vettore b.**/

#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <malloc.h>

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Inserire N & np\n");
    }
    int N = atoi(argv[1]);
    int np = atoi(argv[2]);
    int blockSize = N / np;
    int i, j, k, l;
    printf("block size [%d]", blockSize);
    int **mat = (int **) calloc(N, sizeof(int *));
    int *vetB = (int *) calloc(np, sizeof(int));
    for (i = 0; i < N; i++) {
        mat[i] = (int *) calloc(N, sizeof(int));
        for (j = 0; j < N; j++) {
            mat[i][j] = 1;
        }
    }
    double t0, t1;
    int somma=0;
    t0 = omp_get_wtime();
#pragma omp parallel for private(i,j,k,l) shared(mat,N,np,blockSize) num_threads(np) reduction(+:vetB[:N]) collapse(2)
    for (i = 0; i < N; i +=blockSize) {
        for (j = 0; j < N; j +=blockSize) {
            somma =0;
            for (k = i; k < i+blockSize;k++) {
                for (l =  j; l < j+ blockSize; l++) {
                    vetB[i/blockSize] += mat[k][l];
                }
            }
        }
    }
    t1 = omp_get_wtime();
    printf("tempo per il calcolo [%.16lf] \n", t1 - t0);
    for (i = 0; i < np; i++) {
        printf("vetB[%d] = %d\n", i, vetB[i]);
    }
}
