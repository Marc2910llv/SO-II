#include "bloques.h"
#include "semaforo_mutex_posix.h"
// Pere Joan Vives Morey
// Marc Llobera Villalonga
// Carlos Lozano Alemañy

static sem_t *mutex;
static unsigned int inside_sc = 0;

static int descriptor = 0;

int bmount(const char *camino) // Abrir Fichero
{
    if (descriptor > 0) {
       close(descriptor);
   }

    if (!mutex)
    { // el semáforo es único en el sistema y sólo se ha de inicializar 1 vez (padre)
        mutex = initSem();
        if (mutex == SEM_FAILED)
        {
            return -1;
        }
    }

    umask(000);
    descriptor = open(camino, O_RDWR | O_CREAT, 0666); // obtenemos el desccriptor del fichero
    if (descriptor == -1)
    {
        perror("ERROR AL ABRIR EL FICHERO");
        return -1;
    }
    return descriptor; // devolvemos el descriptor del fichero
}

int bumount() // Cerrar Fichero
{
    descriptor = close(descriptor);
    deleteSem();

    if (close(descriptor) == -1)
    {
        perror("ERROR AL CERRAR EL FICHERO");
        return -1;
    }
    return 0;
}

int bwrite(unsigned int nbloque, const void *buf) // Escribe 1 bloque en el dispositivo virtual, en el bloque físico especificado por nbloque
{
    int desplazamiento = nbloque * BLOCKSIZE;
    lseek(descriptor, desplazamiento, SEEK_SET);               // movemos el puntero del fichero en el offset correcto
    size_t nbytesEscritos = write(descriptor, buf, BLOCKSIZE); // volcar el contenido del buffer (de tamaño BLOCKSIZE) en dicha posición del dispositivo virtual
    if (nbytesEscritos < 0)
    {
        perror("ERROR AL ESCRIBIR EL BLOQUE EN LA POSICIÓN " + nbloque);
        return -1;
    }
    return nbytesEscritos; // devuelve la cantidad de bytes escritos
}

int bread(unsigned int nbloque, void *buf) // Lee 1 bloque del dispositivo virtual, que se corresponde con el bloque físico especificado por nbloque
{
    int desplazamiento = nbloque * BLOCKSIZE;
    lseek(descriptor, desplazamiento, SEEK_SET);            // volcar el contenido del buffer (de tamaño BLOCKSIZE) en dicha posición del dispositivo virtual
    size_t nbytesLeidos = read(descriptor, buf, BLOCKSIZE); // volcar en el buffer (de tamaño BLOCKSIZE) el contenido de los nbytes (BLOCKSIZE) a partir de dicha posición del dispositivo virtual
    if (nbytesLeidos < 0)
    {
        perror("ERROR AL LEER EL BLOQUE DEL DISPOSITIVO VIRTUAL QUE CORRESPONDE A LA POSICIÓN " + nbloque);
        return -1;
    }
    return nbytesLeidos; // devuelve la cantidad de bytes leidos
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
