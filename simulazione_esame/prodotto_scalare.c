//
// Created by nonnt66 on 19/04/24.
//
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int main() {
    int i, N;
    float *a, *b, result = 0;
    time_t t0, t1;
    N = 8000000;
    a = (float *) calloc(N, sizeof(float));
    b = (float *) calloc(N, sizeof(float));

    //inizializzazione
    printf("caricamento vettore\n");
    t0 = clock();
    for (i = 0; i < N; i++) {
        a[i] = (float) 1;
        b[i] = (float) 1;
    }
//    for (i = 0; i < N; i++) {
//        printf("%f\t %f\n", a[i], b[i]);
//    }
    t0 = clock();
    for (int j = 0; j < N; j++) {
        result += a[j] * b[j];
    }
    t1 = clock();
    printf("prodotto scalare: %f\n", result);
    printf("tempo di esecuzione: %.16lf\n", (float) (t1 - t0) / CLOCKS_PER_SEC);

    int t = 8;
    float res[t];
    int nloc, r, step, p = 4;
    result = 0;
    t0 = clock();
#pragma omp parallel for private(i) shared(a,b,N) num_threads(t) reduction(+:result)

        for (i=0; i < N; i++) {
            result += a[i] * b[i];
        }


    t1 = clock();
    printf("prodotto scalare parallelo: %f\n", result);
    printf("tempo di esecuzione parallelo: %.16lf\n", (float) (t1 - t0) / CLOCKS_PER_SEC);


}