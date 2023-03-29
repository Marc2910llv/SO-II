/*
 *   Marc Llobera Villalonga
 */

#include <stdio.h>
#include "ficheros.h"
#define RED "\x1b[31m"

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, RED "Sintaxis: escribir <nombre_dispositivo> <'$(cat fichero)'> <diferentes_inodos>\n");
        fprintf(stderr, RED "Offsets: 9000, 209000, 30725000, 409605000, 480000000 \n");
        fprintf(stderr, RED "Si diferentes_inodos=0 se reserva un solo inodo para todos los offsets\n");
        return FALLO;
    }
    int offset[5] = {9000, 209000, 30725000, 409605000, 480000000}; // offsets

    printf("Longitud texto: %ld \n\n", strlen(argv[2]));

    if (bmount(argv[1]) == FALLO)
    {
        fprintf(stderr, RED "Error main bmount");
        return FALLO;
    }

    int ninodo = reservar_inodo('f', 6);
    if (ninodo == FALLO)
    {
        fprintf(stderr, RED "Error main reservar_inodo");
        return FALLO;
    }
    if (ninodo == -2)
    {
        fprintf(stderr, RED "Error main reservar_inodo (no quedan bloques libres)");
        return FALLO;
    }

    for (int i = 0; i < (sizeof(offset) / sizeof(int)); i++)
    {
        printf("Numero inodo reservado: %d\n", ninodo);
        printf("Offset: %d\n", offset[i]);

        int bytesEscritos = mi_write_f(ninodo, argv[2], offset[i], strlen(argv[2]));
        if (bytesEscritos == FALLO)
        {
            fprintf(stderr, RED "Error main mi_write_f");
            return FALLO;
        }

        struct STAT stat;
        if (mi_stat_f(ninodo, &stat) == FALLO)
        {
            fprintf(stderr, RED "Error main mi_stat_f");
            return FALLO;
        }

        printf("Bytes escritos: %d \n", bytesEscritos);
        printf("stat.tamEnBytesLog = %d\n", stat.tamEnBytesLog);
        printf("stat.numBloquesOcupados = %d\n\n", stat.numBloquesOcupados);

        if (strcmp(argv[3], "0"))
        {
            ninodo = reservar_inodo('f', 6);
            if (ninodo == FALLO)
            {
                fprintf(stderr, RED "Error main reservar_inodo");
                return FALLO;
            }
            if (ninodo == -2)
            {
                fprintf(stderr, RED "Error main reservar_inodo (no quedan bloques libres)");
                return FALLO;
            }
        }
    }

    if (bumount() == FALLO)
    {
        fprintf(stderr, RED "Error main bumount");
    }
}