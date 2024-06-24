// 			Formisano Francesca

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <unistd.h>

int** allocateMatrix(int rows, int cols) {
    int** matrix = (int**)malloc(rows * sizeof(int*));
    if (matrix == NULL) {
        fprintf(stderr, "Errore di allocazione della memoria.\n");
        exit(1);
    }

    for (int i = 0; i < rows; i++) {
        matrix[i] = (int*)malloc(cols * sizeof(int));
        if (matrix[i] == NULL) {
            fprintf(stderr, "Errore di allocazione della memoria.\n");
            exit(1);
        }
    }

    return matrix;
}

int* allocateVector(int size) {
    int* vector = (int*)malloc(size * sizeof(int));
    if (vector == NULL) {
        fprintf(stderr, "Errore di allocazione della memoria.\n");
        exit(1);
    }
    return vector;
}

void fillMatrix(int** matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = (rand() % 21); // valori randomici da 0 a 20
        }
    }
}

void fillMatrixONES(int** matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = 1; 
        }
    }
}

void fillMatrixVALUED(int** matrix, int rows, int cols, int value) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = value;
        }
    }
}

void fillVector(int* vector, int size) {
    for (int i = 0; i < size; i++) {
        vector[i] = (rand() % 21); // valori randomici da 0 a 20
    }
}

void fillVectorONES(int* vector, int size) {
    for (int i = 0; i < size; i++) {
        vector[i] = 1; 
    }
}

void fillVectorVALUED(int* vector, int size, int value) {
    for (int i = 0; i < size; i++) {
        vector[i] = value; 
    }
}

// void printMatrix(int** matrix, int rows, int cols) {
//     for (int i = 0; i < rows; i++) {
//         for (int j = 0; j < cols; j++) {
//             printf("[%6d] ", matrix[i][j]);
//         }
//         printf("\n");
//     }
// }

void printMatrix(int** matrix, int rows, int cols) {
    printf("╔");
    for (int i = 0; i < cols; i++) {
        printf("═════");
        if (i < cols - 1) printf("╦");
    }
    printf("╗\n");

    for (int i = 0; i < rows; i++) {
        printf("║");
        for (int j = 0; j < cols; j++) {
            printf(" %3d ║", matrix[i][j]);
        }
        if (i < rows - 1) {
            printf("\n╠");
            for (int k = 0; k < cols; k++) {
                printf("═════");
                if (k < cols - 1) printf("╬");
            }
            printf("╣\n");
        } else {
            printf("\n╚");
            for (int k = 0; k < cols; k++) {
                printf("═════");
                if (k < cols - 1) printf("╩");
            }
            printf("╝\n");
        }
    }
}

void printVector2(int* vector, int size) {
	for (int i = 0; i < size; i++) {
	 printf("[%6d] ", vector[i]);
	}
	printf("\n");
}

void printVector(int* vector, int size) {
    printf("╔");
    for (int i = 0; i < size; i++) {
        printf("═════");
        if (i < size - 1) printf("╦");
    }
    printf("╗\n║");

    for (int i = 0; i < size; i++) {
        printf(" %3d ║", vector[i]);
    }
    printf("\n╚");
    for (int i = 0; i < size; i++) {
        printf("═════");
        if (i < size - 1) printf("╩");
    }
    printf("╝\n");
}

void freeMatrix(int** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void freeVector(int* vector) {
    free(vector);
}

void printMenu() {
    printf("╔══════════════════════════════════════╗\n");
    printf("║          INSERISCI I PARAMETRI       ║\n");
    printf("╚══════════════════════════════════════╝\n");
}

/*
	Implementare un programma parallelo per l'ambiente multicore con np unità procesanti
	che impieghi la libreria OpenMp. Il programma deve essere organizzato come segue:
	- il core master deve generare due matrici A e B di dimensione MxM
	- i core devono collaborare per sommare tra loro tutte le righe della matrice A in 
	  parallelo e conservare il risultato in un unico vettore b
	- quindi, i core devono calcolare in parallelo il prodotto tra la matrice B ed il 
	  vettore b
	- infine il core master deve stampare il risultato ed il tempo d'esecuzione 
*/

int main() {
    srand(time(NULL));

    int np, M, i, j;
    double t0, t1;

    printMenu();

    printf("Inserisci (M) il numero di righe e numero di colonne delle matrici: ");
    scanf("%d", &M);
    printf("Inserisci (np) il numero di core: ");
    scanf("%d", &np);

    int** A = allocateMatrix(M, M);
    int** B = allocateMatrix(M, M);
	int* b = allocateVector(M);
	int* ris = allocateVector(M);

    fillMatrix(A, M, M);
	fillMatrix(B, M, M);
	
    printf("\nLa matrice A inserita è:\n");
    printMatrix(A, M, M);
    printf("\nLa matrice B inserita è:\n");
    printMatrix(B, M, M);

    t0 = omp_get_wtime();
    #pragma omp parallel num_threads(np) private(i,j) shared(A, B, b, ris, M)
	{
		#pragma omp for
		for(i = 0; i < M; i++){
			b[i] = 0;
			for(j = 0; j < M; j++) {
				//#pragma omp atomic
				b[i] += A[j][i];
			}
		}
						
		#pragma omp for
		for(i = 0; i < M; i++){
			ris[i] = 0;
			for(j = 0; j < M; j++) {
				ris[i] += B[i][j] * b[j];
			}
		}
	}
    
    t1 = omp_get_wtime();

	printf("Il vettore b:\n");
	printVector(b, M);

	printf("Il vettore risultato è:\n"); // utilizzo un'altra stampa per consentire una migliore visualizzazione
	printVector2(ris, M);

    printf("Tempo di esecuzione: %f secondi\n", t1-t0);

    freeMatrix(A, M);
	freeMatrix(B, M);
    freeVector(b);

    return 0;
}




