#include <string.h>
#include "bloques.h"
#include "ficheros_basico.h"

// Primera versión
//  argv[0]="mi_mkfs"
//  argv[1]=nombre_dispositivo
//  argv[2]=nbloques (puede sernos útil la función atoi() para obtener el
//  valor numérico a partir del string)

int main(int argc, char *argv[])
{
    initSB();
    initMB();
    initAI();
    printf("sizeof struct inodo is: %lu\n", sizeof(struct inodo));
    int fd = bmount(argv[1]); // se pasa por parámetro el camino para poder abrir el fichero
    unsigned char *buf[BLOCKSIZE];
    memset(buf, 0, BLOCKSIZE);
    for (int i = 0; i < atoi(argv[2]); i++)
    {
        bwrite(i, buf); // escribimos los nbloques
    }
    bumount(); // cerramos el fichero
}