/*
Implementare un programma parallelo per
l’ambiente multicore con np unità processanti che
impieghi la libreria OpenMP. Il programma deve essere
organizzato come segue: il core master generare una
matrice di dimensione N×M. Quindi, ogni core deve
estrarre la riga della matrice con indice uguale al
proprio identificativo e conservarla in un vettore locale;
successivamente deve sommare gli elementi di tale
vettore e conservare il risultato in un vettore c di
dimensione np. Infine, i core devo collaborare per
individuare in massimo tra gli elementi del vettore c.*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include<omp.h>

int main(int argc, char **argv) {
    if (argc < 4) {
        printf("inserire N , M & np");
        exit(1);
    }
    int N = atoi(argv[1]);
    int M = atoi(argv[2]);
    int np = atoi(argv[3]);
    int **mat = (int **) calloc(N, sizeof(int *));
    int *vetC = (int *) calloc(np, sizeof(int));
    int i, j, id, massimo;
    for (i = 0; i < N; i++) {
        mat[i] = (int *) calloc(M, sizeof(int));
        for (j = 0; j < M; j++) {
            mat[i][j] = 2;
        }
    }
#pragma omp parallel for private(i,j,id) shared(mat,vetC,N,M) num_threads(np)
    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            vetC[omp_get_thread_num()] += mat[j][i];
        }
    }
    massimo=vetC[0];
#pragma omp parallel for private(j) shared(vetC,np) num_threads(np) reduction(max:massimo)
    for (j = 0; j < np; j++) {
        if (vetC[j] > massimo) massimo = vetC[j];
    }

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            printf("%d ",mat[i][j]);
        }
        printf("\n");
    }
    for (j = 0; j < np; j++) {
        printf("vet[%d] = %d\n",j,vetC[j]);
    }
    printf("massimo [%d]\n", massimo);
}
