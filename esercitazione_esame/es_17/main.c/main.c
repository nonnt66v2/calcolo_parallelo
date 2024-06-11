// Implementare un programma parallelo per
// l’ambiente multicore con np unità processanti che
// impieghi la libreria OpenMP. Il programma deve essere
// organizzato come segue: il core master deve leggere
// quattro valori scalari a0, a1, a2, a3, quindi i core devono
// collaborare per costruire un vettore di dimensione 4np i cui elementi sono
// dati dalla somma di tali valori con il
// proprio identificativo.

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char **argv) {
    if (argc < 6) {
        printf("Inserire 4 Scalari e numero di thread\n");
        exit(1);
    }
    int a[4];
    int numT = atoi(argv[5]);
    for (int i = 0; i < 4; i++) {
        a[i] = atoi(argv[i + 1]);
    }
    int i, N = 4 * numT;
    int *vet = (int *) calloc(N, sizeof(int));
    double t0,t1;
    t0 = omp_get_wtime();
    int mod;
#pragma omp parallel for private(i,mod) shared(a,N)  reduction(+:vet[:N]) num_threads(numT)
    for (i = 0; i < N; i++) {
        mod = i%4;
        vet[i] += a[mod] + mod;
    }

    t1 = omp_get_wtime();
    for (i = 0; i < N; i++) {
        printf("vet[%d] = %d\n", i, vet[i]);
    }
    printf("tempo per esecuzione [%.16lf] secondi\n",t1-t0);

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
