// Implementare un programma parallelo per
// l’ambiente multicore con np×np unità processanti che
// impieghi la libreria OpenMP. Il programma deve essere
// organizzato come segue: il core master deve leggereuna matrice A di dimensione M×M. Quindi, ogni core
// deve estrarre un blocco della matrice A di dimensione
// (M/np)×(M/np), conservando i valori in np×np matrici.
// Infine, i core devono collaborare per calcolare la
// somma di tali matrici.

#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <malloc.h>

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Inserire M & np\n");
    }
    int M = atoi(argv[1]);
    int np = atoi(argv[2]);
    int blockSize = M / np;
    printf("block size [%d]", blockSize);
    int **mat = (int **) calloc(M, sizeof(int *));
    int **somma = (int **) calloc(M, sizeof(int *));
    for (int i = 0; i < M; i++) {
        mat[i] = (int *) calloc(M, sizeof(int));
        for (int j = 0; j < M; j++) {
            mat[i][j] = 1;
        }
    }
    for (int i = 0; i < np; i++) {
        somma[i] = (int *) calloc(M, sizeof(int));
        for (int j = 0; j < np; j++) {
            somma[i][j] = 0;
        }
    }
    int i, j, k, l;
    double t0, t1;
    t0 = omp_get_wtime();
#pragma omp parallel for private(i,j,k,l) shared(M,np,blockSize,somma) num_threads(np)
    for (i = 0; i < M; i += blockSize) {
        for (j = 0; j < M; j += blockSize) {
            for (k = i; k < i + blockSize; k++) {
                for (l = j; l < j + blockSize; l++) {
                    #pragma omp atomic
                    somma[i/np][j/np] += mat[k][l];
                }
            }
        }
    }
    // for (i = 0; i < M; i++) {
    //     for (j = 0; j < M; j++) {
    //         printf("%d ", mat[i][j]);
    //     }
    //     printf("\n");
    // }
    // printf("\n\n\n");
    //
    // for (i = 0; i < np; i++) {
    //     for (j = 0; j < np; j++) {
    //         printf("%d ", somma[i][j]);
    //     }
    //     printf("\n");
    // }
    t1 = omp_get_wtime();
    printf("calcolo eseguito in [%.16lf] secondi", t1 - t0);
    return 0;
}
