// Pere Joan Vives Morey
// Marc Llobera Villalonga
// Carlos Lozano Alemañy

#include "directorios.h"

int main(int argc, char const *argv[])
{
    if (argc<3)
    {
        perror("ERROR DE SINTAXIS EN mi_ls.c");
        return -1;
    }

    if (bmount(argv[1]) == -1)
    {
        perror("ERROR EN mi_ls.c AL INTENTAR MONTAR EL DISCO EN EL SISTEMA");
        return -1;
    }

    char buffer[TAMBUFFER];
    memset(buffer, 0, TAMBUFFER);

    int total = mi_dir(argv[2], buffer);
    if (total < 0)
    {
        mostrar_error_buscar_entrada(total);
        return -1;
    }
    if (total > -1)
    {
        printf("Total: %d\n", total);
        printf("Tipo\tModo\tmTime\t\t\tTamaño\tNombre\n");
        printf("--------------------------------------------------------------------------------------------\n");
        printf("%s\n", buffer);
    }

    if (bumount() == -1)
    {
        perror("ERROR EN mi_ls.c AL INTENTAR CERRAR");
    }

    return 0;
}