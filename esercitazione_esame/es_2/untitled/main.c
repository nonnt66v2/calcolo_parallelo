// Implementazione di un algoritmo parallelo (np core)
// per il calcolo dell’operazione c=alpha*a+b, con a,b
// vettori di dimensione N e alpha uno scalare, in
// ambiente openMP

#include <stdio.h>
#include <omp.h>
#include <malloc.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc < 4) {
        perror("fornire la dimensione del vettore");
    }
    int N = atoi(argv[1]);
    int numT = atoi(argv[2]);
    int alpha = atoi(argv[3]);
    int *vetA, *vetB, *vetC;
    vetA = (int *) calloc(N, sizeof(int));
    vetB = (int *) calloc(N, sizeof(int));
    vetC = (int *) calloc(N, sizeof(int));
    for (int i = 0; i < N; i++) {
        vetA[i] = 1;
        vetB[i] = 2;
    }
    int i,mulLoc=0;
    double t0,t1;
    t0 = omp_get_wtime();
#pragma omp parallel for private(i) shared (vetA,vetB,N,alpha) reduction(*:mulLoc) reduction(+:vetC[:N]) num_threads(numT)
    for (i = 0; i < N; i++) {
        mulLoc = alpha * vetA[i];
        vetC[i] +=mulLoc+ vetB[i];
    }

    t1 = omp_get_wtime();
    printf("tempo per esecuzione [%.16lf] secondi\n",t1-t0);
    for (i = 0; i < N; i++) {
        printf("vet[%d] = %d\n", i, vetC[i]);
    }
}
/***
// Implementazione di un algoritmo parallelo (np core)
// per il calcolo dell’operazione c=a+b con a,b vettori di
// dimensione N, in ambiente openMP

#include <stdio.h>
#include <omp.h>
#include <malloc.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc < 3) {
        perror("fornire la dimensione del vettore");
    }
    int N = atoi(argv[1]);
    int numT = atoi(argv[2]);
    int *vetA, *vetB, *vetC;
    vetA = (int *) calloc(N, sizeof(int));
    vetB = (int *) calloc(N, sizeof(int));
    vetC = (int *) calloc(N, sizeof(int));
    for (int i = 0; i < N; i++) {
        vetA[i] = 1;
        vetB[i] = 2;
    }
    int i;
    double t0,t1;
    t0 = omp_get_wtime();
#pragma omp parallel for private(i) shared (vetA,vetB,N) reduction(+:vetC[:N]) num_threads(numT)
    for (i = 0; i < N; i++) {
        vetC[i] += vetA[i] + vetB[i];
    }

    t1 = omp_get_wtime();
    printf("tempo per esecuzione [%.16lf] secondi\n",t1-t0);
    for (i = 0; i < N; i++) {
        printf("vet[%d] = %d\n", i, vetC[i]);
    }
}***/
