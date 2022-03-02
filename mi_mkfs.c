#include <string.h>
#include "bloques.h"

// Primera versión
//  argv[0]="mi_mkfs"
//  argv[1]=nombre_dispositivo
//  argv[2]=nbloques (puede sernos útil la función atoi() para obtener el
//  valor numérico a partir del string)

int main(int argc, char **argv)
{
    int fd = bmount(argv[1]);
    unsigned char *buf;
    memset(buf, 0, BLOCKSIZE);
    for (int i = 0; i < *argv[2]; i++)
    {
        bwrite(i, buf);
    }
    bumount();
}