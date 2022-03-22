#include <string.h>
#include "ficheros_basico.h"

int main(int argc, char *argv[])
{
    unsigned char *buf[BLOCKSIZE];
    memset(buf, 0, BLOCKSIZE);
    bmount(argv[1]);

    for (int i = 0; i < atoi(argv[2]); i++)
    {
        bwrite(i, buf);
    }
    initSB(atoi(argv[2]), atoi(argv[2]) / 4);
    initMB();
    initAI();
    reservar_inodo('d', 7);
    bumount();
}