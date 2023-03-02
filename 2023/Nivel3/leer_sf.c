/*
 *   Marc Llobera Villalonga
 *
 *   Programa de pruebas
 *   leer_sf.c {main()}
 */

#include "ficheros_basico.h"

#define DEBUGN1 0; // Lista enlazada de inodos

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

    printf("sizeof struct superbloque is: %lu\n", sizeof(struct superbloque));
    printf("sizeof struct inodo is: %lu\n\n", sizeof(union _inodo));

#if DEBUGN1
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

    if (bumount() == FALLO)
    {
        perror("Error main bumount");
    }
}