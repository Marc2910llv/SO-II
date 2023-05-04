/*
 *   Marc Llobera Villalonga
 */

#include "directorios.h"

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, RED "./mi_cat <disco> </ruta_fichero>\n" RESET);
        return FALLO;
    }

    if (bmount(argv[1]) == FALLO)
    {
        perror("Error main bmount");
        return FALLO;
    }

    int bytes_leidos = 0;
    int tambuffer = 10 * BLOCKSIZE;
    const char *camino = argv[2];
    char buffer[tambuffer];
    int offset = 0;

    memset(buffer, 0, sizeof(buffer));

    int bytes_leidosAux = mi_read(camino, buffer, offset, tambuffer);
    while (bytes_leidosAux > 0)
    {
        bytes_leidos += bytes_leidosAux;

        write(1, buffer, bytes_leidosAux);

        memset(buffer, 0, sizeof(buffer));
        offset += tambuffer;
        bytes_leidosAux = mi_read(camino, buffer, offset, tambuffer);
    }

    if (bytes_leidos < 0)
    {
        perror("Error main bytes_leidos");
        return FALLO;
    }

    if (bumount() == FALLO)
    {
        perror("Error main bumount");
    }

    char string[128];
    sprintf(string, "\nTotal_leidos %d\n", bytes_leidos);
    write(2, string, strlen(string));
}