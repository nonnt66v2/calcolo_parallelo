/**Implementare un programma parallelo per l’ambiente
multicore con np unità processanti che impieghi la
libreria OpenMP. Il programma deve essere
organizzato come segue: il core master deve generareuna matrice di dimensione N×N, ogni core deve
estrarre N/np righe e calcolare il prodotto puntuale tra
i vettori corrispondenti alle righe estratte.**/

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
    int *vetB = (int *) calloc(N, sizeof(int));
    for (i = 0; i < N; i++) {
        mat[i] = (int *) calloc(N, sizeof(int));
        for (j = 0; j < N; j++) {
            mat[i][j] = 2;
        }
    }
    for (j = 0; j < N; j++) {
        vetB[j] = 1;
    }
    double t0, t1;
    t0 = omp_get_wtime();
#pragma omp parallel for private (i,j) shared(mat,N) reduction(*:vetB[:N]) collapse(2) num_threads(np)
    for(i=0;i<N;i++) {
        for(j=0;j<N;j++) {
            printf("%d\n",omp_get_thread_num());
            vetB[i] *= mat[i][j];
        }
    }

    t1 = omp_get_wtime();
    printf("tempo per il calcolo [%.16lf] \n", t1 - t0);
    for (i = 0; i < N; i++) {
        printf("vetB[%d] = %d\n", i, vetB[i]);
    }
}
