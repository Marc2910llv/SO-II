// Pere Joan Vives Morey
// Marc Llobera Villalonga
// Carlos Lozano Alemañy

#include "directorios.h"

int main(int argc, char const *argv[])
{
    if (argc <4)
    {
        perror("ERROR DE SINTAXIS EN mi_chmod.c");
        return -1;
    }

    if (atoi(argv[2]) > 7)
    {

        perror("ERROR DE PERMISOS EN mi_chmod.c");

        return -1;
    }

    if (bmount(argv[1]) == -1)
    {
        perror("ERROR EN mi_chmod.c AL INTENTAR MONTAR EL DISCO");
        return -1;
    }
    int permisos = atoi(argv[2]);
    int error = mi_chmod(argv[3],permisos);
    if (error < 0)
    {
        mostrar_error_buscar_entrada(error);
        return -1;
    }

    if (bumount() == -1)
    {
        perror("ERROR EN mi_chmod.c AL INTENTAR CERRAR");
    }

    return 0;
}