#include <stdio.h>
#include <omp.h>
#include <math.h>

double f(double x) {
    return cos(x) - pow(x,2);
}

int main() {
    double x[1000];
    int n = 1000;
    int tot = 4;
    double res[tot];
    int id;
    double sum_parz = 0;
    int a = 0;
    int b = 2;
    double sum_tot = 0;
    for (int i = 0; i < n; i++) {
        x[i] = i * ((double)(b-a)/n) ;
    }
#pragma omp parallel shared(x, tot, res), private(id, sum_parz), num_threads(tot)
    {
        id = omp_get_thread_num(); // memorizzo l'indice del thread corrente


        for (int i = id * (n/tot); i < ((id+1) * (n/tot)); i++) {
            sum_parz += f(x[i])*((double)(b-a)/n);
        }
        res[id] = sum_parz; // memorizziamo le somme parziali in un vettore
    }

    // colleziono i risultati locali in un'unica variabile
    for (int j=0; j < tot; j++) {
        sum_tot += res[j];
    }

    printf("Somma totale: %lf\n", sum_tot);

    return 0;
}