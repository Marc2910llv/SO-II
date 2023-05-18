/*
 *   Marc Llobera Villalonga
 */

#include "directorios.h"

int main(int argc, char const *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, RED "Sintaxis: ./mi_link disco /ruta_fichero_original /ruta_enlace\n" RESET);
        return FALLO;
    }

    const char *ruta1 = argv[2];
    if (ruta1[strlen(ruta1) - 1] == '/')
    {
        fprintf(stderr, RED "Error: No se pueden crear enlaces de directorios\n" RESET);
        return FALLO;
    }

    const char *ruta2 = argv[3];
    if (ruta2[strlen(ruta2) - 1] == '/')
    {
        fprintf(stderr, RED "Error: La ruta de enlace tiene que referirse a un fichero\n" RESET);
        return FALLO;
    }

    if (bmount(argv[1]) == FALLO)
    {
        perror("Error main bmount");
        return FALLO;
    }

    int error = mi_link(ruta1, ruta2);
    if (error < FALLO)
    {
        mostrar_error_buscar_entrada(error);
        return FALLO;
    }
    else if (error == FALLO)
    {
        perror("Error main mi_link");
        return FALLO;
    }

    if (bumount() == FALLO)
    {
        perror("Error main bumount");
    }
}