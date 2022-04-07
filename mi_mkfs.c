#include <string.h>
#include "ficheros_basico.h"

int main(int argc, char *argv[])
{
    unsigned char *buf[BLOCKSIZE];
    memset(buf, 0, BLOCKSIZE);
    if (bmount(argv[1]) == -1)
    {
        perror("ERROR EN mi_mkfs AL INTENTAR ABRIR EL FICHERO");
        return -1;
    }

    for (int i = 0; i < atoi(argv[2]); i++)
    {
        bwrite(i, buf);
    }
    initSB(atoi(argv[2]), atoi(argv[2]) / 4);
    initMB();
    initAI();
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
}