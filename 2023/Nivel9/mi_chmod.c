/*
 *   Marc Llobera Villalonga
 */

#include "directorios.h"

int main(int argc, char const *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, RED "Sintaxis: ./mi_chmod <disco> <permisos> </ruta>\n" RESET);
        return FALLO;
    }

    int permisos = atoi(argv[2]);

    if (permisos > 7 || permisos < 0)
    {
        fprintf(stderr, RED "Permisos incorrectos, debe ser nº válido (0-7)\n" RESET);
        return FALLO;
    }

    if (bmount(argv[1]) == FALLO)
    {
        perror("Error main bmount");
        return FALLO;
    }

    int error = mi_chmod(argv[3], permisos);
    if (error < FALLO)
    {
        mostrar_error_buscar_entrada(error);
        return FALLO;
    }
    else if (error == FALLO)
    {
        perror("Error main mi_chmod");
        return FALLO;
    }

    if (bumount() == FALLO)
    {
        perror("ERROR EN mi_ls.c AL INTENTAR CERRAR");
    }
}