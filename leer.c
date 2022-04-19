#include <string.h>
#include "ficheros.h"
#include <time.h>

#define tambuffer 1500

int main(int argc, char *argv[])
{
    char buffer_texto[tambuffer];
    int ninodo;
    int bytesllegits, offset, leidos;
    offset = 0;
    struct inodo inodo;
    struct superbloque SB;
    if (argc < 3)
    {
        printf("Sintaxis : leer <nombre_dispositivo> <ninodo>\n ");
    }
    else
    {
        memset(buffer_texto, 0, tambuffer);
        ninodo = atoi(argv[2]);

        bmount(argv[1]);

        if (bread(0, &SB) == -1)
        {
            perror("ERROR EN leer_sf AL LEER EL SUPERBLOQUE");
            return -1;
        }

        leidos = mi_read_f(ninodo, buffer_texto, offset, tambuffer);
        while (leidos > 0)
        {
            bytesllegits = bytesllegits + leidos;
            write(1, buffer_texto, leidos);
            memset(buffer_texto, 0, tambuffer);
            offset = tambuffer + offset;
            leidos = mi_read_f(ninodo, buffer_texto, offset, tambuffer);
        }
        leer_inodo(ninodo, &inodo);
        fprintf(stderr, "tamEnBytesLog: %d\n", inodo.tamEnBytesLog);
        fprintf(sterr, "bytes llegits: %d\ntamEnBytesLog: %d\n", bytesllegits, inodo.tamEnBytesLog);
        bumount();
        return 1;
    }
}