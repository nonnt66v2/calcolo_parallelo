//Implementare un programma parallelo per
// l’ambiente multicore con nq×np unità processanti
// che
// impieghi la libreria OpenMP
// .
//  Il programma deve
// essere
// organizzato come segue: il
//  core master deve
// costruire
// una matrice A, di dimensione M× N, i cui blocchi sono
// generati casualmente e in parallelo da ogni core
// .

///@guardare \Produttoria


#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char **argv) {
    if (argc < 4) {
        printf("fornire in input rispettivamente dimensione di M,N & num_threads\n");
        exit(-1);
    }
    int N = atoi(argv[1]);
    int M = atoi(argv[2]);
    int numT = atoi(argv[3]);
    int **mat = (int **) calloc(N, sizeof(int *));
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
#pragma omp parallel private(i, j, step, indice) shared(mat,nloc, N, M, R) num_threads(numT)
    {
        r = N % numT;
        nloc = N / numT;
        if (omp_get_thread_num() < r) {
            nloc++;
            step = 0;
        } else {
            step = r;
        }

        for (i = 0; i < nloc; i++) {
            indice = i + nloc * omp_get_thread_num() + step;
            for (j = 0; j < M; j++) {
                mat[indice][j] = (int) rand()%500;
                // mat[indice][j] = 1;
            }
        }
    }
    t1 = omp_get_wtime();
    printf("il caricamento ha richiesto [%.16lf] secondi\n", t1 - t0);
    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            //            mat[i][j] = (int) rand();
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
}
