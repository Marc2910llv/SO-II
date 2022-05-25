// Pere Joan Vives Morey
// Marc Llobera Villalonga
// Carlos Lozano Alemañy
#include "directorios.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        perror("ERROR DE SINTAXIS EN mi_cat.c");
        return -1;
    }
    if (bmount(argv[1]) == -1)
    {
        perror("ERROR EN mi_cat.c AL INTENTAR MONTAR EL DISPOSITIVO");
        return -1;
    }

    int tambuffer = 10 * BLOCKSIZE;
    int bytes_leidos = 0;
    int offset = 0;
    char *camino = argv[2];
    char buffer[tambuffer];

    memset(buffer, 0, sizeof(buffer));

    // Leemos todo el fichero o hasta completar el buffer
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
        mostrar_error_buscar_entrada(bytes_leidos);
        bytes_leidos = 0;
    }

    fprintf(stderr, "Total_leidos: %d\n", bytes_leidos);

    if (bumount() == -1)
    {
        return -1;
    }
    return 0;
}