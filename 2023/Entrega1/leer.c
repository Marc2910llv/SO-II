/*
 *   Marc Llobera Villalonga
 */

#include <stdlib.h>
#include "ficheros.h"

int main(int argc, char const *argv[])
{
    int tambuffer = 1500;

    if (argc != 3)
    {
        fprintf(stderr, "Sintaxis correcta: leer <nombre_dispositivo> <ninodo>\n ");
        return FALLO;
    }

    if (bmount(argv[1]) == FALLO)
    {
        fprintf(stderr, "Error main bmount");
        return FALLO;
    }

    struct superbloque SB;
    if (bread(0, &SB) == FALLO)
    {
        fprintf(stderr, "Error main bread");
        return FALLO;
    }

    int ninodo = atoi(argv[2]);
    int bytesLeidos = 0;
    char buffer_texto[tambuffer];
    int offset = 0; // empezamos en el offset 0

    memset(buffer_texto, 0, tambuffer);
    int leidos = mi_read_f(ninodo, buffer_texto, offset, tambuffer);
    if (leidos == FALLO)
    {
        fprintf(stderr, "Error main mi_read_f (fuera del bucle)");
        return FALLO;
    }

    while (leidos > 0)
    {
        bytesLeidos = bytesLeidos + leidos;
        write(1, buffer_texto, leidos);
        memset(buffer_texto, 0, tambuffer);
        offset = offset + tambuffer;
        leidos = mi_read_f(ninodo, buffer_texto, offset, tambuffer);
        if (leidos == FALLO)
        {
            fprintf(stderr, "Error main mi_read_f (dentro del bucle)");
            return FALLO;
        }
    }

    union _inodo inodo;
    if (leer_inodo(ninodo, &inodo) == FALLO)
    {
        fprintf(stderr, "Error main leer_inodo");
        return FALLO;
    }

    fprintf(stderr, "total_leidos: %d\ntamEnBytesLog: %d\n", bytesLeidos, inodo.tamEnBytesLog);

    if (bumount() == FALLO)
    {
        fprintf(stderr, "Error main bumount");
        return FALLO;
    }
    return EXITO;
}