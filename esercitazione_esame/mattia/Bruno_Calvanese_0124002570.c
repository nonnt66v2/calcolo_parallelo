// Bruno Calvanese 0124002570

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <omp.h>

int main(int argc, char **argv) {
    if (argc < 3) {
        perror("fornire la dimensione del vettore e il numero dei thread");
    }
    int N = atoi(argv[1]);
    int np = atoi(argv[2]);
    int alpha=0;
    int *a, *b, *c;

    a = (int *) calloc(N, sizeof(int));
    b = (int *) calloc(N, sizeof(int));
    c = (int *) calloc(N, sizeof(int));

    //Inserimento dei valori nei vettori a e b
    for (int i = 0; i < N; i++) {
        a[i] = 1;
        b[i] = 1;
    }

    int i;
    double t0,t1;

    t0 = omp_get_wtime();

    //Calcolo parallelo del prodotto scalare tra a e b in alpha
    #pragma omp parallel for private (i) shared (a, b, N) num_threads(np) reduction(+:alpha)
    for (i=0; i < N; i++){
e
        alpha += (a[i] * b[i]);
    }
    int mulLoc = 0;
    //Calcolo parallelo dei valori del vettore c tramite la formula c=(alpha+np)*b
    #pragma omp parallel for private(i) shared (a,b,N,alpha) reduction(*:mulLoc) reduction(+:c[:N]) num_threads(np)
    for (i = 0; i < N; i++) {
        mulLoc = alpha + np;
        c[i] +=mulLoc * b[i];
    }

    t1 = omp_get_wtime();

    //Stampa del vettore c
    for (int i = 0; i < N; i++) {
        printf("c[%d] = %d\n", i, c[i]);
    }

    printf("tempo per esecuzione [%.16lf] secondi\n",t1-t0);

}




