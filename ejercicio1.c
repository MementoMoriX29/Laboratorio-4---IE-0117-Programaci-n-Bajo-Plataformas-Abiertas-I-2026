#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
Encuentra la secuencia de 1s consecutivos mas larga en la
matriz, tratandola como un arreglo lineal (los 1s que
continuan en la siguiente fila cuentan como consecutivos).
Utiliza unicamente aritmetica de punteros , sin sintaxis [].

Params:
- int **matrix : puntero a la matriz cuadrada.
- int size : numero de filas y columnas.
- int *result : donde se almacena la longitud encontrada.
*/
void findLargestLine(int **matrix, int size , int *result) {
    int *ptr_base = *(matrix + 0);
    int total = size * size;
    int max = 0;
    int actual = 0;
    int i;

    for (i = 0; i < total; i++) {
        if (*(ptr_base + i) == 1) {
            actual++;
            if (actual > max) {
                max = actual;
            }       } else {
            actual = 0;
        }
    }
    *result = max;
}


/* PSEUDOCÓDIGO

base    ← dirección del primer elemento
total   ← size × size
max     ← 0
current ← 0

para i desde 0 hasta total-1:
    si elemento[i] == 1:
        current ← current + 1
        si current > max: max ← current
    sino:
        current ← 0

*result ← max

*/

/*
Reserva memoria dinamica para una matriz cuadrada de
enteros de tamano size x size.

Params:
- int ***matrix : triple puntero; al salir , *matrix apunta
  a la matriz creada.
- int size : numero de filas y columnas.
*/
void allocateMatrix(int ***matrix, int size) {
    int i;
    *matrix = malloc(size * sizeof(int *)); // Estamos creando el vector donde cada pos. será la dirección de memoria de una fila en la matriz
    
    **matrix = malloc(size * size * sizeof(int)); // Reserva un único bloque para TODOS los datos de la matriz

    for (i = 1; i < size; i++)
        *(*matrix + i) = **matrix + i * size; 
}

/*
Llena la matriz con valores binarios aleatorios (0 o 1).

Params:
- int **matrix : puntero a la matriz a llenar.
- int size : numero de filas y columnas.
*/
void fillMatrix(int **matrix, int size) {
    int i, j;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            *(*(matrix + i) + j) = rand() % 2;
        }
    }
}

/*
Imprime la matriz en stdout.

Params:
- int **matrix : puntero a la matriz.
- int size : numero de filas y columnas.
*/
void printMatrix(int **matrix, int size) {
    printf("Matriz (%dx%d):\n", size , size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", *(*(matrix+ i) + j));
        }
        printf("\n");
    }
}

/*
Libera la memoria asignada a la matriz.

Params:
- int **matrix : puntero a la matriz a liberar.
- int size : numero de filas y columnas.
*/
void freeMatrix(int **matrix, int size) {
    (void)size;      /* no necesario con bloque único, se mantiene por consistencia */
    free(*matrix);   /* libera el bloque de datos */
    free(matrix);    /* libera el arreglo de punteros */
}

int main(void) {
    int size , largestLine;
    int **matrix=NULL;

    srand(time(NULL));

    printf("Ingrese el tamanio de la matriz: ");
    scanf("%d", &size);

    allocateMatrix(&matrix, size);
    fillMatrix(matrix, size);
    printMatrix(matrix, size);
    findLargestLine(matrix, size, &largestLine);
    printf("El tamanio de la secuencia de 1s mas grande es: %d\n", largestLine);
    freeMatrix(matrix, size);
    return 0;
}
