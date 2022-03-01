#include "bloques.h"

int bmount(const char *camino)
{
    descriptor = open(*camino, O_RDWR | O_CREAT, 0666);
    if (descriptor == -1)
    {
        perror("Error");
        return -1;
    }
    return descriptor;
}

int bumount()
{
    umask(000);
    int x = close(descriptor);
    return x;
}

int bwrite(unsigned int nbloque, const void *buf)
{
    int nbytesEscritos;
    int desplazamiento = nbloque * BLOCKSIZE;
    lseek(descriptor, desplazamiento, SEEK_SET);
    nbytes = write(descriptor, buf, BLOCKSIZE);
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
    nbytes = read(descriptor, buf, BLOCKSIZE);
    if (nbytesLeidos < 0)
    {
        perror("Error");
        return -1;
    }
    return nbytesLeidos;
}

// int open(const char *camino, int oﬂags, mode_t mode);
// int close(int descriptor);
// off_t lseek(int descriptor, off_t desplazamiento, int punto_de_referencia);
// size_t write(int descriptor, const void *buf, size_t nbytes);