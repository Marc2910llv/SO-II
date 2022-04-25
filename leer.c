#include <string.h>
#include "ficheros.h"
#include <stdlib.h>
#define tambuffer 4096

int main(int argc, char const *argv[])
{
    int bytes = tambuffer;
    char buffer[bytes];
    int nombre;
    int bytesllegits, offset, cantidad;
    offset = 0;
    struct inodo inodo;
    if (argc < 3)
    {
        printf("Sintaxis : leer <nombre_dispositivo> <ninodo>\n ");
    }
    else
    {
        memset(buffer, 0, bytes);
        nombre = atoi(argv[2]);

        bmount(argv[1]);

        cantidad = mi_read_f(nombre, buffer, offset, bytes);
        printf("cantidad: %d", cantidad);
        leer_inodo(nombre, &inodo);

        while (cantidad > 0)
        {
            bytesllegits = bytesllegits + cantidad;
            write(1, buffer, cantidad);
            memset(buffer, 0, bytes);
            offset += bytes;
            cantidad = mi_read_f(nombre, buffer, offset, bytes);
            if (cantidad == -1)
            {
                perror("Error");
                return -1;
            }
        }
        leer_inodo(nombre, &inodo);
        fprintf(stderr, "total_bytesleidos: %d\n", bytesllegits);
        fprintf(stderr, "tamEnBytesLog: %d\n\n", inodo.tamEnBytesLog);
        bumount();
        return 0;
    }
}