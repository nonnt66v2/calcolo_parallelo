// Implementare un programma parallelo per l’ambiente
// multicore con np unità processanti che impieghi la
// libreria OpenMP. Il programma deve essere
// organizzato come segue: il core master deve generare
// una matrice A di dimensione N×M. Quindi, i core
// devono collaborare per calcolare il minimo tra gli
// elementi delle colonne di propria competenza e
// conservare tale valore in un vettore c di dimensione M.


#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <malloc.h>
int main(int argc,char** argv) {
    if(argc<3) {
        printf("inserire N & M & num_threads\n");
        exit(-1);
    }
    int N = atoi(argv[1]);
    int M = atoi(argv[2]);
    int numT = atoi(argv[3]);

    int **matA = (int**)calloc(N,sizeof(int*));
    int *vetC = (int*)calloc(M,sizeof(int));
    int i,j;
    for(i=0;i<N;i++) {
        matA[i] = (int*)calloc(M,sizeof(int));
        for(j=0;j<M;j++) {
            matA[i][j] = rand()%100;
        }
    }
    double t0,t1;
    t0 = omp_get_wtime();
    int minimo;
#pragma omp parallel for private(i,j) shared(matA,N,M) num_threads(numT) reduction(min:minimo)
    for(i=0;i<N;i++) {
        minimo = matA[0][i];
        for(j=0;j<M;j++) {
            if(matA[j][i] < minimo ) {
                minimo = matA[j][i];
            }
        }
        vetC[i] = minimo;
    }
    t1 = omp_get_wtime();
    printf("il minimo di ogni colonna è stato trovato in [%.16lf] secondi\n",t1-t0);
    for(i=0;i<N;i++) {
        for(j=0;j<M;j++) {
            printf("%d\t",matA[i][j]);
        }
        printf("\n");
    }
    for(j=0;j<M;j++) {
        printf("vet[%d] = %d \n",j,vetC[j]);
    }
}
