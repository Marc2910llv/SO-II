/*
 *   Marc Llobera Villalonga
 */

#include "directorios.h"

int main(int argc, char const *argv[])
{
    if (argc != 5)
    {
        fprintf(stderr, RED "./mi_escribir <disco> </ruta_fichero> <texto> <offset>\n" RESET);
        return FALLO;
    }

    if (bmount(argv[1]) == FALLO)
    {
        perror("Error main bmount");
        return FALLO;
    }

    if ((argv[2][strlen(argv[2]) - 1]) == '/')
    {
        fprintf(stderr, RED "La ruta no es un fichero\n" RESET);
        return FALLO;
    }

    fprintf(stderr, "longitud texto: %ld\n", strlen(argv[3]));

    int bytes_escritos = mi_write(argv[2], argv[3], atoi(argv[4]), strlen(argv[3]));
    if (bytes_escritos < FALLO)
    {
        fprintf(stderr, "Bytes escritos: %d\n", 0);
        return FALLO;
    }
    else if (bytes_escritos == FALLO)
    {
        perror("Error main mi_write");
        fprintf(stderr, "Bytes escritos: %d\n", 0);
        return FALLO;
    }

    if (bumount() == FALLO)
    {
        perror("Error main bumount");
    }

    fprintf(stderr, "Bytes escritos: %d\n", bytes_escritos);
}