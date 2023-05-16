/*
 *   Marc Llobera Villalonga
 */

#include "bloques.h"
#include "semaforo_mutex_posix.h"

static int descriptor = 0;

static sem_t *mutex;
static unsigned int inside_sc = 0;

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
        perror("Error bmount");
        return FALLO;
    }

    if (!mutex)
    { // el semáforo es único en el sistema y sólo se ha de inicializar 1 vez (padre)
        mutex = initSem();
        if (mutex == SEM_FAILED)
        {
            return FALLO;
        }
    }

    return descriptor;
}

/// @brief desmonta el dispositivo virtual
/// @return EXITO o FALLO
int bumount()
{
    if (close(descriptor) == FALLO)
    {
        perror("Error bumount");
        return FALLO;
    }

    deleteSem();

    return EXITO;
}

/// @brief escribe 1 bloque en el dispositivo virtual
/// @param nbloque posicion del bloque que se quiere escribir dentro del dispositivo
/// @param buf puntero que lleva al contenido que se quiere escribir
/// @return bytes escritos o FALLO
int bwrite(unsigned int nbloque, const void *buf)
{
    // Movemos el puntero del fichero a la posicion correcta(depende de nbloque)
    if (lseek(descriptor, nbloque * BLOCKSIZE, SEEK_SET) == FALLO)
    {
        perror("Error bwrite lseek");
        return FALLO;
    }

    // Escribimos 1 bloque con el contenido del buffer
    size_t bytesEscritos = write(descriptor, buf, BLOCKSIZE);
    if (bytesEscritos < 0)
    {
        perror("Error brwite write");
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
    if (lseek(descriptor, nbloque * BLOCKSIZE, SEEK_SET) == FALLO)
    {
        perror("Error bread lseek");
        return FALLO;
    }

    // Leemos 1 bloque y metemos el contenido en el buffer
    size_t bytesLeidos = read(descriptor, buf, BLOCKSIZE);
    if (bytesLeidos < 0)
    {
        perror("Error bread write");
        return FALLO;
    }

    return bytesLeidos;
}

void mi_waitSem()
{
    if (!inside_sc)
    { // inside_sc==0
        waitSem(mutex);
    }
    inside_sc++;
}

void mi_signalSem()
{
    inside_sc--;
    if (!inside_sc)
    {
        signalSem(mutex);
    }
}