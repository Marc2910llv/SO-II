// Pere Joan Vives Morey
// Marc Llobera Villalonga
// Carlos Lozano Alemañy
#include "directorios.h"

int main(int argc, char const *argv[])
{
    if (argc != 5)
    {
        perror("ERROR DE SINTAXIS EN mi_escribir.c");
        return -1;
    }

    if ((argv[2][strlen(argv[2]) - 1]) == '/')
    {
        perror("ERROR EN mi_escribir.c, NO SE TRATA DE UN FICHERO");
        return -1;
    }

    if (bmount(argv[1]) == -1)
    {
        perror("ERROR EN mi_escribir.c AL INTENTAR MONTAR EL DISPOSITIVO VIRTUAL");
        return -1;
    }

    int bytes_escritos = mi_write(argv[2], argv[3], atoi(argv[4]), strlen(argv[3]));
    if (bytes_escritos < 0)
    {
        mostrar_error_buscar_entrada(bytes_escritos);
        bytes_escritos = 0;
    }

    if (bumount() == -1)
    {
        return -1;
    }
    fprintf(stderr, "Bytes escritos: %d\n", bytes_escritos);
    return 0;
}