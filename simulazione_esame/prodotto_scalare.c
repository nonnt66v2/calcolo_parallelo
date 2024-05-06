//
// Created by nonnt66 on 19/04/24.
//
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <math.h>
#include "limits.h"

int main() {
    int i, N = pow(12,8);

    float *a, *b, result = 0;
    double t0, t1;
    a = (float *) malloc(N * sizeof(float));
    b = (float *) malloc(N * sizeof(float));

    //inizializzazione
    printf("caricamento vettore\n");
    for (i = 0; i < N; i++) {
        a[i] = (float) 1;
        b[i] = (float) 1;
    }
//    for (i = 0; i < N; i++) {
//        printf("%f\t %f\n", a[i], b[i]);
//    }
    t0 = omp_get_wtime();
    for (int j = 0; j < N; j++) {
        result += a[j] * b[j];
    }
    t1 = omp_get_wtime();
    printf("prodotto scalare: %f\n", result);
    printf("tempo di esecuzione: %.16lf\n", (float) (t1 - t0));

    int t = 12;
    float res[t];
    int nloc, r, step = 0, p = 4, id_thread;
    result = 0;
    t0 = omp_get_wtime();
#pragma omp parallel for private(i) shared(a, b, N) num_threads(t) reduction(+:result)

    for (i = 0; i < N; i++) {
        result += a[i] * b[i];
    }


    t1 = omp_get_wtime();
    printf("prodotto scalare parallelo: %f\n", result);
    printf("tempo di esecuzione parallelo: %.16lf\n", (float) (t1 - t0));


}