/*
 *   Marc Llobera Villalonga
 */

#include "ficheros.h"

int main(int argc, char const *argv[])
{
    int ninodo = atoi(argv[2]);
    int permisos = atoi(argv[3]);
    if (argc != 4)
    {
        printf("Sintaxis: permitir <nombre_dispositivo> <ninodo> <permisos>\n");
        return FALLO;
    }

    if (bmount(argv[1]) == FALLO)
    {
        perror("Error main bmount");
        return FALLO;
    }

    if (mi_chmod_f(ninodo, permisos) == FALLO)
    {
        perror("Error main mi_chmod_f");
        return FALLO;
    }

    if (bumount() == FALLO)
    {
        perror("Error main bumount");
    }
}