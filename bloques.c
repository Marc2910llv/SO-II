#include "bloques.h"

int bmount(const char *camino) //abrir fichero
{
    umask(000);
    descriptor = open(camino, O_RDWR | O_CREAT);
    if (descriptor == -1)
    {
        perror("Error");
        return -1;
    }
    return descriptor;
}

int bumount() //para cerrar el fichero
{
    int x = close(descriptor);
    return x; //retorna un 0 si se ha cerrado correctamente, -1 en caso de error
}

int bwrite(unsigned int nbloque, const void *buf)
{
    int desplazamiento = nbloque * BLOCKSIZE;
    lseek(descriptor, desplazamiento, SEEK_SET);
    int nbytesEscritos = write(descriptor, buf, BLOCKSIZE);
    if (nbytesEscritos < 0)
    {
        perror("Error");
        return -1;
    }
    return nbytesEscritos;
}

int bread(unsigned int nbloque, void *buf)
{
    int nbytesLeidos;
    int desplazamiento = nbloque * BLOCKSIZE;
    lseek(descriptor, desplazamiento, SEEK_SET);
    nbytesLeidos = read(descriptor, buf, BLOCKSIZE);
    if (nbytesLeidos < 0)
    {
        perror("Error");
        return -1;
    }
    return nbytesLeidos;
}