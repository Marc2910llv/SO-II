#include <string.h>
#include "ficheros_basico.h"
#include <time.h> //esta librería incluirla en ficheros_basico.h

int main(int argc, char *argv[])
{
    bmount(argv[1]);
    // void *punter;
    struct superbloque *SB;
    int j = bread(0, punter);
    // SB = punter;
    // if (j == -1)
    // {
    // perror("Error");
    // return -1;
    // }
    // printf("DATOS DEL SUPERBLOQUE\n");
    // printf("posPrimerBloqueMB = %d\n", (*SB).posPrimerBloqueMB);
    // printf("posUltimoBloqueMB = %d\n", (*SB).posUltimoBloqueMB);
    // printf("posPrimerBloqueAI = %d\n", (*SB).posPrimerBloqueAI);
    // printf("posUltimoBloqueAi = %d\n", (*SB).posUltimoBloqueAI);
    // printf("posPrimerBloqueDatos = %d\n", (*SB).posPrimerBloqueDatos);
    // printf("posUltimoBloqueDatos = %d\n", (*SB).posUltimoBloqueDatos);
    // printf("posInodoRaiz = %d\n", (*SB).posInodoRaiz);
    // printf("posPrimerInodoLibre = %d\n", (*SB).posPrimerInodoLibre);
    // printf("cantBloquesLibres = %d\n", (*SB).cantBloquesLibres);
    // printf("cantInodosLibres = %d\n", (*SB).cantInodosLibres);
    // printf("totBloques = %d\n", (*SB).totBloques);
    // printf("totInodos = %d\n", (*SB).totInodos);
    // printf("Sizeof struct superbloque = %ld\n", sizeof(*SB));
    // printf("Sizeof struct Inodo = %ld\n", sizeof(struct inodo));

    // printf("\nsizeof struct superbloque: %ld\n", sizeof(struct superbloque));
    // printf("sizeof struct inodo:  %ld\n", sizeof(struct inodo));

    // struct tm *ts;
    // char atime[80];
    // char mtime[80];
    // char ctime[80];

    // struct inodo inodo;
    // int ninodo;

    // leer_inodo(ninodo, &inodo);
    // ts = localtime(&inodo.atime);
    // strftime(atime, sizeof(atime), "%a %Y-%m-%d %H:%M:%S", ts);
    // ts = localtime(&inodo.mtime);
    // strftime(mtime, sizeof(mtime), "%a %Y-%m-%d %H:%M:%S", ts);
    // ts = localtime(&inodo.ctime);
    // strftime(ctime, sizeof(ctime), "%a %Y-%m-%d %H:%M:%S", ts);
    // printf("ID: %d ATIME: %s MTIME: %s CTIME: %s\n", ninodo, atime, mtime, ctime);

    int posInodoReservado = reservar_inodo('f', 6);
    bread(posSB, &SB);

    traducir_bloque_inodo(posInodoReservado, 8, 1);
    traducir_bloque_inodo(posInodoReservado, 204, 1);
    traducir_bloque_inodo(posInodoReservado, 30004, 1);
    traducir_bloque_inodo(posInodoReservado, 400004, 1);
    traducir_bloque_inodo(posInodoReservado, 468750, 1);

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

    bumount();
}
//-----------------Nivel 4----------------------
// En este nivel leer_sf.c ya no es necesario reservar y liberar un bloque, ni mostrar el mapa de bits ni el directorio raíz
//(no borréis las sentencias, tan sólo ponerlas como comentarios). Ahora habrá que añadir a leer_sf.c las instrucciones necesarias
// para comprobar la traducción de bloques de diferentes rangos de punteros.
// Para ello necesitaremos previamente reservar artificialmente un inodo con la función reservar_inodo(),
// y llamaremos a la función traducir_bloque_inodo() para traducir los bloques lógicos siguientes: 8, 204, 30.004, 400.004 y 468.750
// En la función traducir_bloque_inodo() de ficheros_basico.c poner los fprintf() necesarios,
// justo después de la asignación de un valor a ptr (en las 4 situaciones posibles) para mostrar:
// el nivel de punteros
// el índice para ese nivel
// y los bloques físicos reservados para bloques de punteros y bloques de datos
