/*
Implementare un programma parallelo per l’ambiente
multicore con np unità processanti che impieghi la
libreria OpenMP. Il programma deve essere
organizzato come segue: il core master deve leggere
una matrice A di dimensione N×N ed uno scalare alpha.
Quindi, i core devono collaborare per calcolare il
prodotto tra lo scalare alpha e la matrice A, sfruttano
una strategia di parallelizzazione che usi la
distribuzione per colonne della matrice A come
decomposizione del dominio.
*/


#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <malloc.h>

int main(int argc, char **argv) {
 if (argc < 3) {
  printf("Inserire N & np\n");
 }
 int N = atoi(argv[1]);
 int np = atoi(argv[2]);
 int scalare = atoi(argv[3]);
 int i, j;
 int **mat = (int **) calloc(N, sizeof(int *));
 for (i = 0; i < N; i++) {
  mat[i] = (int *) calloc(N, sizeof(int));
  for (j = 0; j < N; j++) {
   mat[i][j] = 2;
  }
 }
 double t0, t1;
 t0 = omp_get_wtime();
 int prod=1;
#pragma omp parallel for private(i,j) shared(N,mat) num_threads(np) reduction(*:prod)collapse(2)
 for(i=0;i<N;i++) {

  for(j=0;j<N;j++) {
   prod=1;
   prod*=scalare * mat[j][i];
   mat[j][i] = prod;
  }
 }
 t1 = omp_get_wtime();
 for(i=0;i<N;i++) {
  for(j=0;j<N;j++) {
   printf("%d ",mat[i][j]);
  }
  printf("\n");
 }
 printf("tempo per il calcolo [%.16lf] \n", t1 - t0);


}