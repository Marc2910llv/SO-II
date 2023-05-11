/*
 *   Marc Llobera Villalonga
 */

#include "directorios.h"

int main(int argc, char const *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, RED "Sintaxis: ./mi_touch <disco> <permisos> </ruta>\n" RESET);
        return FALLO;
    }

    if (atoi(argv[2]) < 0 || atoi(argv[2]) > 7)
    {
        fprintf(stderr, RED "Permisos incorrectos, debe ser nº válido (0-7)\n" RESET);
        return FALLO;
    }

    const char *ruta = argv[3];
    if (ruta[0] != '/')
    {
        fprintf(stderr, RED "Error: Camino incorrecto.\n" RESET);
        return FALLO;
    }

    if (ruta[strlen(ruta) - 1] == '/')
    {
        fprintf(stderr, RED "Error: No es fichero\n" RESET);
        return FALLO;
    }

    if (bmount(argv[1]) == FALLO)
    {
        perror("Error main bmount");
        return FALLO;
    }

    int error = mi_creat(ruta, atoi(argv[2]));
    if (error == FALLO)
    {
        perror("Error main mi_creat");
    }
    else if (error < FALLO)
    {
        mostrar_error_buscar_entrada(error);
    }

    if (bumount() == FALLO)
    {
        perror("Error main bumount");
    }
}