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
    if (((nbloques / 8) % BLOCKSIZE) > 0)
    {
        return (((nbloques / 8) / BLOCKSIZE) + 1);
    }
    return ((nbloques / 8) / BLOCKSIZE);
}

/// @brief tamaño en bloques del array de inodos
/// @param ninodos
/// @return número de bloques para el array de inodos
int tamAI(unsigned int ninodos)
{
    if (((ninodos * INODOSIZE) % BLOCKSIZE) > 0)
    {
        return (((ninodos * INODOSIZE) / BLOCKSIZE) + 1);
    }
    return ((ninodos * INODOSIZE) / BLOCKSIZE);
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
    SB.totInodos = ninodos;

    if (bwrite(posSB, &SB) == FALLO)
    {
        perror("Error initSB");
    }
}

/// @brief iniciar el mapa de bits
/// @return
int initMB()
{
    unsigned char bufferMB[BLOCKSIZE];
    memset(bufferMB, 0, BLOCKSIZE);

    struct superbloque SB;
    if (bread(posSB, &SB) == FALLO)
    {
        perror("Error initMB bread");
        return FALLO;
    }

    // Calculamos cuantos bloques ocuparán los metadatos
    int nbloquesMB = tamSB + tamMB(SB.totBloques) + tamAI(SB.totInodos);

    // Ponemos los bits correspondientes a 1
    if (nbloquesMB % 8 == 0)
    {
        for (int i = 0; i < nbloquesMB / 8; i++)
        {
            bufferMB[i] = 255;
        }
    }
    else // Si no ocupan bloques exactos tendremos que hacer un desplazamiento
    {
        for (int i = 0; i <= nbloquesMB / 8; i++)
        {
            bufferMB[i] = 255;
        }

        bufferMB[nbloquesMB / 8] = bufferMB[nbloquesMB / 8] << (8 - (nbloquesMB % 8));
    }

    // Escribimos el mapa de bits modificado
    if (bwrite(SB.posPrimerBloqueMB, bufferMB) == FALLO)
    {
        perror("Error initMB bwrite (MB)");
        return FALLO;
    }

    // Actualizamos la cantidad de bloques libres que quedan
    SB.cantBloquesLibres = SB.cantBloquesLibres - nbloquesMB;
    if (bwrite(posSB, &SB) == FALLO)
    {
        perror("Error initMB bwrite (SB)");
    }
}

/// @brief iniciar lista de inodos libres
/// @return
int initAI()
{
    struct superbloque SB;
    if (bread(posSB, &SB) == FALLO)
    {
        perror("Error initAI bread (SB)");
        return FALLO;
    }

    union _inodo inodos[BLOCKSIZE / INODOSIZE];

    int contInodos = SB.posPrimerInodoLibre + 1;                       // si hemos inicializado SB.posPrimerInodoLibre = 0
    for (int i = SB.posPrimerBloqueAI; i <= SB.posUltimoBloqueAI; i++) // para cada bloque del AI
    {
        for (int j = 0; j < BLOCKSIZE / INODOSIZE; j++) // para cada inodo del AI
        {
            inodos[j].tipo = 'l'; // libre

            if (contInodos < SB.totInodos) // si no hemos llegado al último inodo
            {
                inodos[j].punterosDirectos[0] = contInodos; // enlazamos con el siguiente
                contInodos++;
            }
            else // hemos llegado al último inodo
            {
                inodos[j].punterosDirectos[0] = UINT_MAX;
                // hay que salir del bucle, el último bloque no tiene por qué estar completo !!!
                break;
            }
        }

        if (bwrite(i, &inodos) == FALLO)
        {
            perror("Error initAI bwrite (inodo)");
        }
    }
}