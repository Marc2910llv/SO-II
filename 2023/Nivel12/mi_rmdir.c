/*
 *   Marc Llobera Villalonga
 */

#include "directorios.h"

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, RED "Sintaxis: ./mi_rm disco /ruta\n" RESET);
        return FALLO;
    }

    const char *ruta = argv[2];
    if (ruta[strlen(ruta) - 1] != '/')
    {
        fprintf(stderr, RED "Error: No es directorio\n" RESET);
        return FALLO;
    }

    if (bmount(argv[1]) == FALLO)
    {
        perror("Error main bmount");
        return FALLO;
    }

    int error = mi_unlink(ruta);
    if (error < FALLO)
    {
        mostrar_error_buscar_entrada(error);
        return FALLO;
    }
    else if (error == FALLO)
    {
        perror("Error main mi_unlink");
        return FALLO;
    }

    if (bumount() == FALLO)
    {
        perror("Error main bumount");
    }
}