// Pere Joan Vives Morey
// Marc Llobera Villalonga
// Carlos Lozano Alemañy
#include "directorios.h"

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        perror("ERROR EN mi_link.c, SINTAXIS ERRÓNEA");
        return -1;
    }

    if (argv[2][strlen(argv[2]) - 1] == '/')
    {
        perror("ERROR EN mi_link.c, LA RUTA DEL FICHERO ORIGINAL NO ES UN FICHERO");
        return -1;
    }
    if (argv[3][strlen(argv[3]) - 1] == '/')
    {
        perror("ERROR EN mi_link.c, LA RUTA DE ENLACE NO ES UN FICHERO");
        return -1;
    }

    if (bmount(argv[1]) == -1)
    {
        perror("ERROR EN mi_link.c AL MONTAR EL DISPOSITIVO VIRTUAL");
        return -1;
    }

    if (mi_link(argv[2], argv[3]) < 0)
    {
        return -1;
    }

    bumount();
    return 0;
}