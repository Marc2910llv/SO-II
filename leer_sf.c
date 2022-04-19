#include <string.h>
#include "ficheros_basico.h"
#include <time.h> //esta librería incluirla en ficheros_basico.h

int main(int argc, char *argv[])
{
    bmount(argv[1]);
    void *punter;
    int bloque_reservado;
    struct superbloque SB;
    if (bread(0, &SB) == -1)
    {
        perror("ERROR EN leer_sf AL LEER EL SUPERBLOQUE");
        return -1;
    }
    int posbyte, posbit, nbloqueMB, nbloqueabs;

    printf("DATOS DEL SUPERBLOQUE\n");
    printf("posPrimerBloqueMB = %d\n", SB.posPrimerBloqueMB);
    printf("posUltimoBloqueMB = %d\n", SB.posUltimoBloqueMB);
    printf("posPrimerBloqueAI = %d\n", SB.posPrimerBloqueAI);
    printf("posUltimoBloqueAi = %d\n", SB.posUltimoBloqueAI);
    printf("posPrimerBloqueDatos = %d\n", SB.posPrimerBloqueDatos);
    printf("posUltimoBloqueDatos = %d\n", SB.posUltimoBloqueDatos);
    printf("posInodoRaiz = %d\n", SB.posInodoRaiz);
    printf("posPrimerInodoLibre = %d\n", SB.posPrimerInodoLibre);
    printf("cantBloquesLibres = %d\n", SB.cantBloquesLibres);
    printf("cantInodosLibres = %d\n", SB.cantInodosLibres);
    printf("totBloques = %d\n", SB.totBloques);
    printf("totInodos = %d\n", SB.totInodos);
    printf("Sizeof struct superbloque = %ld\n", sizeof(SB));
    printf("Sizeof struct Inodo = %ld\n", sizeof(struct inodo));

    printf("RESERVAMOS UN BLOQUE Y LUEGO LO LIBERAMOS \n");
    bloque_reservado = reservar_bloque();
    if (bread(0, &SB); == -1)
    {
        perror("ERROR EN leer_sf AL LEER EL SUPERBLOQUE");
        return -1;
    }
    printf("Se ha reservado el bloque físico %d que era el primero libre \n", bloque_reservado);
    printf("cantBloquesLibres = %d\n", SB.cantBloquesLibres);

    printf("MAPA DE BITS CON BLOQUES DE METADATOS OCUPADOS \n");
    int valor = leer_bit(0);
    printf("leer_bit(0): %i \n", valor);
    valor = leer_bit(1);
    printf("leer_bit(1): %i \n", valor);
    valor = leer_bit(13);
    printf("leer_bit(13): %i \n", valor);
    valor = leer_bit(14);
    printf("leer_bit(14): %i \n", valor);
    valor = leer_bit(3138);
    printf("leer_bit(3138): %i \n", valor);
    valor = leer_bit(3139);
    printf("leer_bit(3139): %i \n", valor);
    valor = leer_bit(99999);
    printf("leer_bit(99999): %i \n", valor);

    if (bumount() == -1)
    {
        perror("ERROR EN leer_sf AL INTENTAR CERRAR EL FICHERO");
        return -1;
    }
}
