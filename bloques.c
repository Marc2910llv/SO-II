#include "bloques.h"

int bmount(const char *camino) // Abrir Fichero
{
    umask(000);
    descriptor = open(camino, O_RDWR | O_CREAT, 0666); // obtenemos el desccriptor del fichero
    if (descriptor == -1)
    {
        perror("Error");
        return -1;
    }
    return descriptor; // devolvemos el descriptor del fichero
}

int bumount() // Cerrar Fichero
{
    int x = close(descriptor);
    return x; // retorna un 0 si se ha cerrado correctamente, -1 en caso de error
}

int bwrite(unsigned int nbloque, const void *buf) // Escribe 1 bloque en el dispositivo virtual, en el bloque físico especificado por nbloque
{
    int desplazamiento = nbloque * BLOCKSIZE;
    lseek(descriptor, desplazamiento, SEEK_SET);               // movemos el puntero del fichero en el offset correcto
    size_t nbytesEscritos = write(descriptor, buf, BLOCKSIZE); // volcar el contenido del buffer (de tamaño BLOCKSIZE) en dicha posición del dispositivo virtual
    if (nbytesEscritos < 0)
    {
        perror("Error");
        return -1;
    }
    return nbytesEscritos; // devuelve la cantidad de bytes escritos
}

int bread(unsigned int nbloque, void *buf)
{
    size_t nbytesLeidos;
    int desplazamiento = nbloque * BLOCKSIZE;
    lseek(descriptor, desplazamiento, SEEK_SET);     // volcar el contenido del buffer (de tamaño BLOCKSIZE) en dicha posición del dispositivo virtual
    nbytesLeidos = read(descriptor, buf, BLOCKSIZE); //  volcar en el buffer (de tamaño BLOCKSIZE) el contenido de los nbytes (BLOCKSIZE) a partir de dicha posición del dispositivo virtual
    if (nbytesLeidos < 0)
    {
        perror("Error");
        return -1;
    }
    return nbytesLeidos; // devuelve la cantidad de bytes leidos
}