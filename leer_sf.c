#include <string.h>
#include "ficheros_basico.h"
#include <time.h> //esta librería incluirla en ficheros_basico.h

int main(int argc, char *argv[])
{
    int fd = bmount(argv[1]);
    void *punter;
    struct superbloque *SB;
    int j = bread(0, punter);
    SB = punter;
    if (j == -1)
    {
        perror("Error");
        return -1;
    }
    printf("DATOS DEL SUPERBLOQUE\n");
    printf("posPrimerBloqueMB = %d\n", (*SB).posPrimerBloqueMB);
    printf("posUltimoBloqueMB = %d\n", (*SB).posUltimoBloqueMB);
    printf("posPrimerBloqueAI = %d\n", (*SB).posPrimerBloqueAI);
    printf("posUltimoBloqueAi = %d\n", (*SB).posUltimoBloqueAI);
    printf("posPrimerBloqueDatos = %d\n", (*SB).posPrimerBloqueDatos);
    printf("posUltimoBloqueDatos = %d\n", (*SB).posUltimoBloqueDatos);
    printf("posInodoRaiz = %d\n", (*SB).posInodoRaiz);
    printf("posPrimerInodoLibre = %d\n", (*SB).posPrimerInodoLibre);
    printf("cantBloquesLibres = %d\n", (*SB).cantBloquesLibres);
    printf("cantInodosLibres = %d\n", (*SB).cantInodosLibres);
    printf("totBloques = %d\n", (*SB).totBloques);
    printf("totInodos = %d\n", (*SB).totInodos);
    printf("Sizeof struct superbloque = %ld\n", sizeof(*SB));
    printf("Sizeof struct Inodo = %ld\n", sizeof(struct inodo));

    struct tm *ts;
    char atime[80];
    char mtime[80];
    char ctime[80];

    struct inodo inodo;
    int ninodo;

    leer_inodo(ninodo, &inodo);
    ts = localtime(&inodo.atime);
    strftime(atime, sizeof(atime), "%a %Y-%m-%d %H:%M:%S", ts);
    ts = localtime(&inodo.mtime);
    strftime(mtime, sizeof(mtime), "%a %Y-%m-%d %H:%M:%S", ts);
    ts = localtime(&inodo.ctime);
    strftime(ctime, sizeof(ctime), "%a %Y-%m-%d %H:%M:%S", ts);
    printf("ID: %d ATIME: %s MTIME: %s CTIME: %s\n", ninodo, atime, mtime, ctime);
}

 - -gs (/// MRROBOT)
