//
// Created by nonnt66 on 08/04/24.
// Numero Ideale di Thread: 2
//
#include <stdio.h>
#include <stdio.h>
#include <omp.h>
#include <math.h>

int main() {
    const int N = 100000;
    int vet[100000];
    vet[0] = 1;
    //allocazione vettore con malloc
    for(int i=1; i<N; i++){
        vet[i] = i;
    }
    int r=0;
    int somma=0;
    int step;
    int sommaTot = 0;
    const int t = 8;
    int indice;
    int res[t];

    int nloc ;
    double t0,t1;
    t0 = omp_get_wtime();
#pragma omp parallel private(somma, nloc,res,step) shared(sommaTot,r) num_threads(t)
    {
        r = N%t;
        nloc = N / t;
        if(omp_get_thread_num()<r){
            nloc++;
            step=0;
        }
        else{
            step = r;
        }
        int id = omp_get_thread_num();
        for (int i = 0; i < nloc; i++) {
//            indice = i+nloc*id+step;
//            res[id] += vet[indice];
            somma=somma+vet[i+nloc*id+step];
        }
//        sommaTot += res[id];
          sommaTot += somma;
    }
    t1 = omp_get_wtime();
printf("Tempo: %.16lf\n", t1-t0);
    printf("Somma: %d\n", sommaTot);
}