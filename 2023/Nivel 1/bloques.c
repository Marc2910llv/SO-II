/*
 *   Marc Llobera Villalonga
 *
 *   bloques.c {bmount(), bumount(), bwrite(), bread()}
 */

#include "bloques.h"

static int descriptor = 0;

/// @brief monta el dispositivo virtual
/// @param camino
/// @return descriptor del fichero o FALLO
int bmount(const char *camino)
{
    umask(000);
    // Abrimos el fichero con RDWR(lectura y escriptura) y CREAT(crea el fichero si este no existe)
    // Permisos 0666(rw-rw-rw-)
    descriptor = open(camino, O_RDWR | O_CREAT, 0666);
    if (descriptor < 0)
    {
        perror("Error");
        return FALLO;
    }

    return descriptor;
}

/// @brief desmonta el dispositivo virtual
/// @return EXITO o FALLO
int bumount()
{
    if (close(descriptor) == -1)
    {
        perror("Error");
        return FALLO;
    }

    return EXITO;
}

/// @brief escribe 1 bloque en el dispositivo virtual
/// @param nbloque posicion del bloque que se quiere escribir dentro del dispositivo
/// @param buf puntero que lleva al contenido que se quiere escribir
/// @return bytes escritos o FALLO
int bwrite(unsigned int nbloque, const void *buf)
{
    // Movemos el puntero del fichero a la posicion correcta(depende de nbloque)
    if (lseek(descriptor, nbloque * BLOCKSIZE, SEEK_SET) == -1)
    {
        perror("Error");
        return FALLO;
    }

    // Escribimos 1 bloque con el contenido del buffer
    int bytesEscritos = write(descriptor, buf, BLOCKSIZE);
    if (bytesEscritos < 0)
    {
        perror("Error");
        return FALLO;
    }

    return bytesEscritos;
}

/// @brief lee 1 bloque del dispositivo virtual
/// @param nbloque posicion del bloque que se quiere leer dentro del dispositivo
/// @param buf puntero que contendrá el bloque leído
/// @return bytes leídos o FALLO
int bread(unsigned int nbloque, void *buf)
{
    // Movemos el puntero del fichero a la posicion correcta(depende de nbloque)
    if (lseek(descriptor, nbloque * BLOCKSIZE, SEEK_SET) == -1)
    {
        perror("Error");
        return FALLO;
    }

    // Leemos 1 bloque y metemos el contenido en el buffer
    int bytesLeidos = write(descriptor, buf, BLOCKSIZE);
    if (bytesLeidos < 0)
    {
        perror("Error");
        return FALLO;
    }

    return bytesLeidos;
}