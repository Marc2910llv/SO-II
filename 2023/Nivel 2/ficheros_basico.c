/*
 *   Marc Llobera Villalonga
 *
 *   ficheros_basico.c {tamMB(), tamAI(), initSB(), initMB(), initAI()}
 */

#include "ficheros_basico.h"

/// @brief tamaño en bloques del mapa de bits
/// @param nbloques
/// @return número de bloques para el mapa de bits
int tamMB(unsigned int nbloques)
{
    if (((nbloques / 8) % BLOCKSIZE) == 0)
    {
        return (nbloques / 8) / BLOCKSIZE;
    }
    return ((nbloques / 8) / BLOCKSIZE) + 1;
}

/// @brief tamaño en bloques del array de inodos
/// @param ninodos
/// @return número de bloques para el array de inodos
int tamAI(unsigned int ninodos)
{
    if (((ninodos * INODOSIZE) % BLOCKSIZE) == 0)
    {
        return (ninodos * INODOSIZE) / BLOCKSIZE;
    }
    return ((ninodos * INODOSIZE) / BLOCKSIZE) + 1;
}

/// @brief iniciar los datos del superbloque
/// @param nbloques
/// @param ninodos
/// @return
int initSB(unsigned int nbloques, unsigned int ninodos)
{
    struct superbloque SB;

    SB.posPrimerBloqueMB = posSB + tamSB; // posSB = 0, tamSB = 1
    SB.posUltimoBloqueMB = SB.posPrimerBloqueMB + tamMB(nbloques) - 1;
    SB.posPrimerBloqueAI = SB.posUltimoBloqueMB + 1;
    SB.posUltimoBloqueAI = SB.posPrimerBloqueAI + tamAI(ninodos) - 1;
    SB.posPrimerBloqueDatos = SB.posUltimoBloqueAI + 1;
    SB.posUltimoBloqueDatos = nbloques - 1;
    SB.posInodoRaiz = 0;
    SB.posPrimerInodoLibre = 0;
    SB.cantBloquesLibres = nbloques;
    SB.cantInodosLibres = ninodos;
    SB.totBloques = nbloques;
    SB.totinodos = ninodos;

    if (bwrite(posSB, &SB) = FALLO)
    {
        perror("Error initSB");
    }
}

int initMB() {}
int initAI() {}