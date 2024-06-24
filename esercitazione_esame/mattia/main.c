///SENNECA MARIO MATTIA 0124002478

#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

//Funzione per allocare una matrice MxM
int **allocaMatrice(int m) {
    int i,j;
    int **matrice = (int **) malloc(m * sizeof(int*));
    for (i = 0; i < m; i++) {
        matrice[i] = (int *) malloc(m * sizeof(int));
        for (j = 0; j < m; j++) {
            matrice[i][j] = (rand() % 9) + 1;
        }
    }
    return matrice;
}

//Procedura per il popolamento randomico di una matrice
void popolaMatrice(int **matrice, int m) {
    int i, j;
    for (i = 0; i < m; i++) {
        for (j = 0; j < m; j++) {
            matrice[i][j]  = (rand() % 9) + 1;
        }
    }
}


// Procedura per leggere una matrice
void leggiMatrice(int **matrice, int m) {
    int i, j;
    for (i = 0; i < m; i++) {
        for (j = 0; j < m; j++) {
            printf("[%d]\t", matrice[i][j]);
        }
        printf("\n");
    }
}

// Procedura per leggere un vettore
void leggiVettore(int *vettore, int m) {
    int i;
    for (i = 0; i < m; i++) {
        printf("[%d]\t", vettore[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    int m, np, i, j;
    double tf, ti, t;

    printf("Inserisci il size (m):\n");
    scanf("%d", &m);

    printf("Inserisci il numero di processi (np):\n");
    scanf("%d", &np);


    ///IL CORE MASTER DEVE GENERARE DUE MATRICI A e B DI DIMENSIONE MxM.

    // Allocamento delle matrici A e B e allocamento dei vettori b e prodotti
    int **A = allocaMatrice(m);
    int **B = allocaMatrice(m);
    int *b = (int *) malloc(m * sizeof(int));
    int *prodotti = (int *) malloc(m * sizeof(int));

#pragma omp master
    {
        popolaMatrice(A, m);
        popolaMatrice(B, m);
    }

    printf("\nMatrice A:\n");
    leggiMatrice(A, m);

    printf("\nMatrice B:\n");
    leggiMatrice(B, m);

    // inizio a prendere il tempo
    ti = omp_get_wtime();

    /// I CORE DEVONO COLLABORARE PER SOMMARE TRA LORO TUTTE LE RIGHE DELLA MATRICE A IN PARALLELO E CONSERVARE IL RISULTATO IN UNICO VETTORE b

#pragma omp parallel for shared(A) private(i,j) num_threads(np) reduction(+:b[:m])
    for (i = 0; i < m; i++) {
        for (j = 0; j < m; j++) {
            b[i] += A[j][i];
        }
    }

    printf("\nVettore b:\n");
    leggiVettore(b, m);

    ///QUINDI, I CORE DEVONO CALCOLARE IN PARALLELO IL PRODOTTO TRA LA MATRICE B ED IL VETTORE b
    int prodotto=0;
#pragma omp parallel for shared(B) private(i,j) num_threads(np) reduction(+:prodotto)
    for (i = 0; i < m; i++) {
        for (j = 0; j < m; j++) {
            prodotto += B[i][j] * b[j];
        }

        prodotti[i] = prodotto;
    }

    // prendo il fine tempo
    tf = omp_get_wtime();

    // calcolo del tempo totale
    t = tf - ti;

    ///INFINE, IL CORE MASTER DEVE STAMPARE IL RISULTATO ED IL TEMPO D'ESECUZIONE
#pragma omp master
    {
        printf("\nVettore Prodotti:\n");
        leggiVettore(prodotti, m);
        printf("\nTempo totale: %lf\n", t);
    }
}
