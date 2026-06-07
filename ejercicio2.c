#include <stdio.h>
#include <stdlib.h>

/* * Lee una imagen PGM (formato P2) desde un archivo.
 * Reserva memoria dinamica para los pixeles.
 */
unsigned char *read_pgm(const char *filename, int *width, int *height, int *max_val) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: No se pudo abrir el archivo %s\n", filename);
        return NULL;
    }

    char format[3];
    // Leer el formato que debe ser P2
    if (fscanf(file, "%2s", format) != 1 || format[0] != 'P' || format[1] != '2') {
        printf("Error: El formato no es P2\n");
        fclose(file);
        return NULL;
    }

    // Leer el encabezado: ancho, alto y el valor maximo
    if (fscanf(file, "%d %d %d", width, height, max_val) != 3) {
        printf("Error: Encabezado invalido\n");
        fclose(file);
        return NULL;
    }

    int total = (*width) * (*height);
    unsigned char *pixels = (unsigned char *)malloc(total * sizeof(unsigned char));
    if (pixels == NULL) {
        printf("Error: No se pudo asignar memoria para los pixeles\n");
        fclose(file);
        return NULL;
    }

    // Leer cada pixel del archivo de texto
    for (int i = 0; i < total; i++) {
        int val;
        if (fscanf(file, "%d", &val) != 1) {
            printf("Error al leer el pixel en la posicion %d\n", i);
            free(pixels);
            fclose(file);
            return NULL;
        }
        // Guardamos el valor usando aritmetica de punteros
        *(pixels + i) = (unsigned char)val;
    }

    fclose(file);
    return pixels;
}

/* * Aplica un umbral T al arreglo de pixeles in_place.
 * Usa unicamente aritmetica de punteros.
 */
void apply_threshold(unsigned char *pixels, int total, int threshold) {
    for (int i = 0; i < total; i++) {
        // RESTRICCION: Sin corchetes, usamos *(pixels + i)
        if (*(pixels + i) >= threshold) {
            *(pixels + i) = 255;
        } else {
            *(pixels + i) = 0;
        }
    }
}

/* * Genera el negativo de un arreglo de pixeles en un nuevo arreglo.
 * Reserva memoria dinamica para el resultado.
 * Usa unicamente aritmetica de punteros.
 */
unsigned char *make_negative(unsigned char *pixels, int total) {
    unsigned char *neg_pixels = (unsigned char *)malloc(total * sizeof(unsigned char));
    if (neg_pixels == NULL) {
        printf("Error: No se pudo asignar memoria para el negativo\n");
        return NULL;
    }

    for (int i = 0; i < total; i++) {
        // RESTRICCION: Leemos de pixels y escribimos en neg_pixels con punteros
        *(neg_pixels + i) = 255 - *(pixels + i);
    }

    return neg_pixels;
}

/* * Escribe un arreglo de pixeles como imagen PGM (formato P2).
 */
void write_pgm(const char *filename, unsigned char *pixels, int width, int height, int max_val) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: No se pudo crear el archivo de salida %s\n", filename);
        return;
    }

    // Escribir el encabezado reglamentario P2
    fprintf(file, "P2\n");
    fprintf(file, "%d %d\n", width, height);
    fprintf(file, "%d\n", max_val);

    int total = width * height;
    // Escribir los pixeles
    for (int i = 0; i < total; i++) {
        fprintf(file, "%d", *(pixels + i));
        
        // Formato estetico: saltar de linea cada 15 pixeles para que no sea una sola linea infinita
        if ((i + 1) % 15 == 0) {
            fprintf(file, "\n");
        } else {
            fprintf(file, " ");
        }
    }

    fprintf(file, "\n");
    fclose(file);
}

/* * Calcula e imprime estadisticas del arreglo original y del arreglo umbralizado.
 */
void print_stats(unsigned char *original, unsigned char *thresholded, int total) {
    int white_count = 0;
    int black_count = 0;
    double sum_original = 0.0;

    for (int i = 0; i < total; i++) {
        // Sumar para el promedio del original
        sum_original += *(original + i);

        // Contar blancos y negros en el ya umbralizado
        if (*(thresholded + i) == 255) {
            white_count++;
        } else if (*(thresholded + i) == 0) {
            black_count++;
        }
    }

    double average = sum_original / total;

    printf("\n=== ESTADISTICAS DE LA IMAGEN ===\n");
    printf("Valor promedio de los pixeles originales: %.2f\n", average);
    printf("Cantidad de pixeles blancos (255): %d\n", white_count);
    printf("Cantidad de pixeles negros (0): %d\n", black_count);
}

int main(void) {
    int width = 0, height = 0, max_val = 0, threshold = 0;
    unsigned char *pixels = NULL;
    unsigned char *thresholded_pixels = NULL;
    unsigned char *negative_pixels = NULL;

    // 1. Lectura de la imagen original
    pixels = read_pgm("input.pgm", &width, &height, &max_val);
    if (pixels == NULL) {
        return 1; // Terminar si no encuentra el archivo de entrada
    }

    printf("Imagen leida con exito: %dx%d, Maximo valor = %d\n", width, height, max_val);

    // Solicitar el umbral T al usuario
    printf("Ingrese el valor del umbral T: ");
    if (scanf("%d", &threshold) != 1) {
        printf("Error al leer el umbral.\n");
        free(pixels);
        return 1;
    }

    int total_pixels = width * height;

    // Duplicar el arreglo original para aplicar el umbral in-place sin perder el original para las estadisticas
    thresholded_pixels = (unsigned char *)malloc(total_pixels * sizeof(unsigned char));
    if (thresholded_pixels == NULL) {
        free(pixels);
        return 1;
    }
    for (int i = 0; i < total_pixels; i++) {
        *(thresholded_pixels + i) = *(pixels + i);
    }

    // 2. Aplicar filtro de umbral
    apply_threshold(thresholded_pixels, total_pixels, threshold);

    // 3. Generar el negativo a partir de la umbralizada
    negative_pixels = make_negative(thresholded_pixels, total_pixels);
    if (negative_pixels == NULL) {
        free(pixels);
        free(thresholded_pixels);
        return 1;
    }

    // 4. Escritura de los archivos finales
    write_pgm("output_threshold.pgm", thresholded_pixels, width, height, max_val);
    write_pgm("output_negative.pgm", negative_pixels, width, height, max_val);
    printf("Archivos de salida guardados correctamente.\n");

    // 5. Imprimir estadisticas
    print_stats(pixels, thresholded_pixels, total_pixels);

    // Liberacion total de memoria para asegurar 0 memory leaks en Valgrind
    free(pixels);
    free(thresholded_pixels);
    free(negative_pixels);

    return 0;
}
