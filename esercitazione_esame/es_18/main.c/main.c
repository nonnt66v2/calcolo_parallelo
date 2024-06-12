// Implementare un programma parallelo per
// l’ambiente multicore con np unità processanti che
// impieghi la libreria OpenMP. Il programma deve essere
// organizzato come segue: il core master deve leggere un
// vettore a, di dimensione N ed uno scalare beta. Quindi
// i core devo collaborare per verificare se nel vettore
// esista almeno un elemento uguale al valore beta.

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <malloc.h>
int main(int argc, char **argv) {
    if(argc < 4 ) {
        printf("fornire N, beta e num_threads\n");
        exit(-1);
    }
    int N = atoi(argv[1]);
    int beta = atoi(argv[2]);
    int numT = atoi(argv[3]);
    int *vet = (int*)calloc(N,sizeof(int));
    for(int i=0;i<N;i++) {
        printf("inserisci vet[%d]: ",i);
        scanf("%d",&vet[i]);
    }
    int i,trovato = 0;
    double t0,t1;
    t0 = omp_get_wtime();
#pragma omp parallel for private(i) shared (vet,N,beta,trovato) num_threads(numT)
    for(i=0;i<N;i++) {
        if(vet[i] == beta) trovato = 1;
    }

    t1 = omp_get_wtime();
    if(trovato == 1) printf("elemento trovato in [%.16lf] secondi",t1-t0);
    else printf("elemento NON trovato in [%.16lf] secondi",t1-t0);
}
