// Pere Joan Vives Morey
// Marc Llobera Villalonga
// Carlos Lozano Alemañy
#include <string.h>
#include "ficheros_basico.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        perror("ERROR DE SINTAXIS EN mi_mkfs.c");
        return -1;
    }

    unsigned char buf[BLOCKSIZE];

    memset(buf, 0, BLOCKSIZE);

    if (bmount(argv[1]) == -1)
    {
        perror("ERROR EN mi_mkfs AL INTENTAR ABRIR EL FICHERO");
        return -1;
    }

    for (int i = 0; i < atoi(argv[2]); i++)
    {
        if (bwrite(i, buf) == -1)
        {
            perror("ERROR EN mi_mkfs.c AL ESCRIBIR EN EL DISPOSITIVO VIRTUAL");
            return -1;
        }
    }

    if (initSB(atoi(argv[2]), (atoi(argv[2]) / 4)) == -1)
    {
        perror("ERROR EN mi_mkfs.c AL GENERAR EL SUPERBLOQUE");
        return -1;
    }
    if (initMB() == -1)
    {
        perror("ERROR EN mi_mkfs.c AL GENERAR EL MAPA DE BITS");
        return -1;
    }
    if (initAI() == -1)
    {
        perror("ERROR EN mi_mkfs.c AL GENERAR EL ARRAY DE INODOS");
        return -1;
    }

    if (reservar_inodo('d', 7) == -1)
    {
        perror("ERROR EN mi_mkfs AL INTENTAR RESERVAR UN INODO");
        return -1;
    }

    if (bumount() == -1)
    {
        perror("ERROR EN mi_mkfs AL INTENTAR CERRAR EL FICHERO");
        return -1;
    }
    return 0;
}