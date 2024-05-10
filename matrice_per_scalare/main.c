//
// Created by nonnt66 on 04/04/24.
//
#include <stdio.h>
#include <omp.h>
#include <math.h>
#include <malloc.h>

//

int main() {
    const int N = 1024;
    int matA[N][N];
    int blockSize=2;
    int **matDinA = (int **) malloc(N* sizeof(int));
    //alloca una matrice dinamica
    printf("carica matrice\n");
    double t0 = omp_get_wtime();
    for (int i = 0; i < N; i++) {
        matDinA[i] = (int *) malloc(N * sizeof(int));
    }
    double t1 = omp_get_wtime();
    printf("tempo caricamento: %.16lf\n", t1 - t0);
    int scalare = 2;
    int i, j;
    //prodotto matrice per scalare

    int vetR[N];
    printf("prodotto matrice per scalare\n");
    t0 = omp_get_wtime();
#pragma omp parallel for private(i, j) num_threads(2)
    for (int blockRow=0; blockRow < N ; blockRow++) {
        for (int blockCol =0; blockCol< N; blockCol++) {
            for (int k = blockRow; k < blockRow+blockSize && k<N; k++) {
                for (int l = blockCol; l < blockCol+blockSize && l<N; l++) {
                    matDinA[k][l]*=scalare;

                }
            }
        }
    }
    t1 = omp_get_wtime();
    printf("tempo prodotto: %.16lf\n", t1 - t0);


    return 0;

}