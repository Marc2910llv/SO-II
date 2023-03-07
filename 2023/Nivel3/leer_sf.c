/*
 *   Marc Llobera Villalonga
 *
 *   Programa de pruebas
 *   leer_sf.c {main()}
 */

#include "ficheros_basico.h"

#define DEBUGN1 0 // Lista enlazada de inodos

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        perror("Error Sintaxis");
        return FALLO;
    }

    if (bmount(argv[1]) == FALLO)
    {
        perror("Error main bmount");
        return FALLO;
    }

    struct superbloque SB;
    if (bread(posSB, &SB) == FALLO)
    {
        perror("Error main bread (SB)");
        return FALLO;
    }

    printf("DATOS DEL SUPERBLOQUE\n");
    printf("posPrimerBloqueMB = %d\n", SB.posPrimerBloqueMB);
    printf("posUltimoBloqueMB = %d\n", SB.posUltimoBloqueMB);
    printf("posPrimerBloqueAI = %d\n", SB.posPrimerBloqueAI);
    printf("posUltimoBloqueAI = %d\n", SB.posUltimoBloqueAI);
    printf("posPrimerBloqueDatos = %d\n", SB.posPrimerBloqueDatos);
    printf("posUltimoBloqueDatos = %d\n", SB.posUltimoBloqueDatos);
    printf("posInodoRaiz = %d\n", SB.posInodoRaiz);
    printf("posPrimerInodoLibre = %d\n", SB.posPrimerInodoLibre);
    printf("cantBloquesLibres = %d\n", SB.cantBloquesLibres);
    printf("cantInodosLibres = %d\n", SB.cantInodosLibres);
    printf("totBloques = %d\n", SB.totBloques);
    printf("totInodos = %d\n\n", SB.totInodos);

#if DEBUGN1
    printf("sizeof struct superbloque is: %lu\n", sizeof(struct superbloque));
    printf("sizeof struct inodo is: %lu\n\n", sizeof(union _inodo));

    printf("RECORRIDO LISTA ENLAZADA DE INODOS LIBRES\n");
    union _inodo inodos[BLOCKSIZE / INODOSIZE];
    int nlibres = 0;
    for (int i = SB.posPrimerBloqueAI; i <= SB.posUltimoBloqueAI; i++)
    {
        if (bread(i, &inodos) == FALLO)
        {
            perror("Error main bread (inodo)");
            return FALLO;
        }

        for (int j = 0; j < BLOCKSIZE / INODOSIZE; j++)
        {
            if ((inodos[j].tipo == 'l'))
            {
                nlibres++;
                printf("%d ", nlibres);
            }
        }
    }
#endif

    printf("\nRESERVAMOS UN BLOQUE Y LUEGO LO LIBERAMOS:\n");
    int reservado = reservar_bloque(); // Actualiza el SB
    if (reservado == FALLO)
    {
        perror("Error main reservar_bloque");
        return FALLO;
    }
    if (reservado == -2)
    {
        perror("Error main reservar_bloque, no hay bloques libres");
    }
    if (bread(posSB, &SB) == FALLO)
    { // Actualizar los valores del SB
        perror("Error main bread (SB)");
        return FALLO;
    }
    printf("Se ha reservado el bloque físico nº %i que era el 1º libre indicado por el MB.\n", reservado);
    printf("SB.cantBloquesLibres: %i\n", SB.cantBloquesLibres);
    if (liberar_bloque(reservado) == FALLO)
    {
        perror("Error main liberar_bloque");
        return FALLO;
    }
    if (bread(posSB, &SB) == FALLO)
    { // Actualizar los valores del SB
        perror("Error main bread (SB)");
        return FALLO;
    }
    printf("Liberamos ese bloque, y después SB.cantBloquesLibres: %i\n\n", SB.cantBloquesLibres);

    printf("\nMAPA DE BITS CON BLOQUES DE METADATOS OCUPADOS\n");
    int bit = leer_bit(posSB);
    if (bit == FALLO)
    {
        perror("Error main leer_bit");
        return FALLO;
    }
    // printf("[leer_bit(%i) -> posbyte: %i, posbit: %i, nbloqueMB: %i, nbloqueabs: %i)]", bit, )
    printf("posSB: %i --> leer_bit(%i) = %i\n", posSB, posSB, bit);

    bit = leer_bit(SB.posPrimerBloqueMB);
    if (bit == FALLO)
    {
        perror("Error main leer_bit");
        return FALLO;
    }
    printf("SB.posPrimerBloqueMB: %i --> leer_bit(%i) = %i\n", SB.posPrimerBloqueMB, SB.posPrimerBloqueMB, bit);

    bit = leer_bit(SB.posUltimoBloqueMB);
    if (bit == FALLO)
    {
        perror("Error main leer_bit");
        return FALLO;
    }
    printf("SB.posUltimoBloqueMB: %i --> leer_bit(%i) = %i\n", SB.posUltimoBloqueMB, SB.posUltimoBloqueMB, bit);

    bit = leer_bit(SB.posPrimerBloqueAI);
    if (bit == FALLO)
    {
        perror("Error main leer_bit");
        return FALLO;
    }
    printf("SB.posPrimerBloqueAI: %i --> leer_bit(%i) = %i\n", SB.posPrimerBloqueAI, SB.posPrimerBloqueAI, bit);

    bit = leer_bit(SB.posUltimoBloqueAI);
    if (bit == FALLO)
    {
        perror("Error main leer_bit");
        return FALLO;
    }
    printf("SB.posUltimoBloqueAI: %i --> leer_bit(%i) = %i\n", SB.posUltimoBloqueAI, SB.posUltimoBloqueAI, bit);

    bit = leer_bit(SB.posPrimerBloqueDatos);
    if (bit == FALLO)
    {
        perror("Error main leer_bit");
        return FALLO;
    }
    printf("SB.posPrimerBloqueDatos: %i --> leer_bit(%i) = %i\n", SB.posPrimerBloqueDatos, SB.posPrimerBloqueDatos, bit);

    bit = leer_bit(SB.posUltimoBloqueDatos);
    if (bit == FALLO)
    {
        perror("Error main leer_bit");
        return FALLO;
    }
    printf("SB.posUltimoBloqueDatos: %i --> leer_bit(%i) = %i\n", SB.posUltimoBloqueDatos, SB.posUltimoBloqueDatos, bit);

    printf("\nDATOS DEL DIRECTORIO RAIZ\n");
    struct tm *ts;
    char atime[80];
    char mtime[80];
    char ctime[80];
    union _inodo inodo;
    int ninodo = 0; // el directorio raiz es el inodo 0
    if (leer_inodo(ninodo, &inodo) == FALLO)
    {
        perror("Error main leer_inodo");
        return FALLO;
    }
    ts = localtime(&inodo.atime);
    strftime(atime, sizeof(atime), "%a %Y-%m-%d %H:%M:%S", ts);
    ts = localtime(&inodo.mtime);
    strftime(mtime, sizeof(mtime), "%a %Y-%m-%d %H:%M:%S", ts);
    ts = localtime(&inodo.ctime);
    strftime(ctime, sizeof(ctime), "%a %Y-%m-%d %H:%M:%S", ts);
    printf("tipo: %c\n", inodo.tipo);
    printf("permisos: %i\n", inodo.permisos);
    printf("ATIME: %s \nMTIME: %s \nCTIME: %s\n", atime, mtime, ctime);
    printf("nlinks: %i\n", inodo.nlinks);
    printf("tamEnBytesLog: %i\n", inodo.tamEnBytesLog);
    printf("numBloquesOcupados: %i\n", inodo.numBloquesOcupados);

    if (bumount() == FALLO)
    {
        perror("Error main bumount");
    }
}