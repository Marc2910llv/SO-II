/*
 *   Marc Llobera Villalonga
 *
 *   Programa de pruebas
 */

#include "ficheros.h"

#define tambuffer 1500 // tamaño que vamos a leer

int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        printf("Sintaxis : leer <nombre_dispositivo> <ninodo>\n ");
        return FALLO;
    }

    if (bmount(argv[1]) == FALLO)
    {
        perror("Error main bmount");
        return FALLO;
    }

    int ninodo = atoi(argv[2]);
    size_t bytesLeidos = 0;
    char buffer_texto[tambuffer];
    int offset = 0; // empezamos en el offset 0

    memset(buffer_texto, 0, tambuffer);
    size_t leidos = mi_read_f(ninodo, buffer_texto, offset, tambuffer);
    if (leidos == FALLO)
    {
        perror("Error main mi_read_f (fuera del bucle)");
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
            perror("Error main mi_read_f (dentro del bucle)");
            return FALLO;
        }
    }

    union _inodo inodo;
    if (leer_inodo(ninodo, &inodo) == FALLO)
    {
        perror("Error main leer_inodo");
        return FALLO;
    }

    fprintf(stderr, "total_leidos: %ld\n", bytesLeidos);
    fprintf(stderr, "tamEnBytesLog: %d\n\n", inodo.tamEnBytesLog);

    if (bumount() == FALLO)
    {
        perror("Error main bumount");
    }
}
