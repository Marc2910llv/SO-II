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
        return FALLO;
    }

    return EXITO;
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
        return FALLO;
    }

    return EXITO;
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
            return FALLO;
        }
    }

    return EXITO;
}

/// @brief escribir 0 o 1 en un bit del MB que representa un bloque
/// @param nbloque
/// @param bit valor del bit que queremos actualizar (0 o 1)
/// @return
int escribir_bit(unsigned int nbloque, unsigned int bit)
{
    struct superbloque SB;
    if (bread(posSB, &SB) == FALLO)
    {
        perror("Error escribir_bit bread (SB)");
        return FALLO;
    }

    int posbyte = nbloque / 8;
    int posbit = nbloque % 8;

    int nbloqueMB = posbyte / BLOCKSIZE;

    int nbloqueabs = SB.posPrimerBloqueMB + nbloqueMB;

    unsigned char bufferMB[BLOCKSIZE];
    if (bread(nbloqueabs, bufferMB) == FALLO)
    {
        perror("Error escribir_bit bread (bloque)");
        return FALLO;
    }

    posbyte = posbyte % BLOCKSIZE;

    unsigned char mascara = 128; // 10000000
    mascara >>= posbit;          // desplazamientode bits a la derecha

    if (bit == 1)
    {
        bufferMB[posbyte] |= mascara; //  operador OR parabits
    }
    else // bit == 0
    {
        bufferMB[posbyte] &= ~mascara; // operadores AND y NOT para bits
    }

    if (bwrite(nbloqueabs, bufferMB) == FALLO)
    {
        perror("Error escribir_bit bwrite (bloque)");
        return FALLO;
    }

    return EXITO;
}

/// @brief leer un determinado bit del MB devolviendo su valor
/// @param nbloque
/// @return valor del bit leído o FALLO
char leer_bit(unsigned int nbloque)
{
    struct superbloque SB;
    if (bread(posSB, &SB) == FALLO)
    {
        perror("Error leer_bit bread (SB)");
        return FALLO;
    }

    int posbyte = nbloque / 8;
    int posbit = nbloque % 8;

    int nbloqueMB = posbyte / BLOCKSIZE;

    int nbloqueabs = SB.posPrimerBloqueMB + nbloqueMB;

    unsigned char bufferMB[BLOCKSIZE];
    if (bread(nbloqueabs, bufferMB) == FALLO)
    {
        perror("Error leer_bit bread (bloque)");
        return FALLO;
    }

    posbyte = posbyte % BLOCKSIZE;

    unsigned char mascara = 128;  // 10000000
    mascara >>= posbit;           // desplazamiento debits a la derecha
    mascara &= bufferMB[posbyte]; // operador AND parabits
    mascara >>= (7 - posbit);     // desplazamiento debits a la derecha

    return mascara;
}

/// @brief poner el bit del MB que representa el primer bloque libre
/// @return nbloque reservado, -2 (no quedan bloques libres) o FALLO
int reservar_bloque()
{
    struct superbloque SB;
    if (bread(posSB, &SB) == FALLO)
    {
        perror("Error reservar_bloque bread (SB)");
        return FALLO;
    }

    if (SB.cantBloquesLibres < 1)
    {
        return -2;
    }

    // Localizamos la posición en el dispositivo virtual, del primer
    // bloque del MB que tenga algún bit a 0, nbloqueabsy lo leemos
    unsigned char bufferMB[BLOCKSIZE];
    unsigned char bufferAux[BLOCKSIZE];
    memset(bufferAux, 255, BLOCKSIZE);
    unsigned int nbloqueabs = SB.posPrimerBloqueMB;

    for (nbloqueabs; nbloqueabs <= SB.posUltimoBloqueMB; nbloqueabs++)
    {
        if (bread(nbloqueabs, bufferMB) == FALLO)
        {
            perror("Error reservar_bloque bread (iterar bloques)");
            return FALLO;
        }
        if (memcmp(bufferMB, bufferAux, BLOCKSIZE) != 0)
        {
            break;
        }
    }

    // Localizamos qué bytedentro de ese bloque tiene algún 0
    int posbyte = 0;
    while (posbyte < BLOCKSIZE && bufferMB[posbyte] == 255)
    {
        posbyte++;
    }

    // Localizamos el primer bitdentro de ese byte que vale 0
    unsigned char mascara = 128; // 10000000
    int posbit = 0;
    // encontrar el primer bit a 0 en ese byte
    while (bufferMB[posbyte] & mascara)
    {                            // operadorAND para bits
        bufferMB[posbyte] <<= 1; // desplazamientode bits a la izquierda
        posbit++;
    }

    // Reservamos el bloque físico
    int nbloque = ((nbloqueabs - SB.posPrimerBloqueMB) * BLOCKSIZE + posbyte) * 8 + posbit;
    if (escribir_bit(nbloque, 1) == FALLO)
    {
        perror("Error reservar_bloque escribir_bit");
        return FALLO;
    }

    SB.cantBloquesLibres--;
    if (bwrite(posSB, &SB) == FALLO)
    {
        perror("Error reservar_bloque bwrite (SB)");
        return FALLO;
    }

    // Limpiamos el bloque de la zona de datos
    memset(bufferAux, 0, BLOCKSIZE);
    if (bwrite(SB.posPrimerBloqueDatos + nbloque - 1, bufferAux) == -1)
    {
        perror("Error reservar_bloque bwrite (limpiar bloque)");
        return FALLO;
    }

    return nbloque;
}

/// @brief liberar un bloque determinado
/// @param nbloque
/// @return nbloque liberado
int liberar_bloque(unsigned int nbloque) {}

/// @brief escribir el contenido pasado por parámetro en un determinado inodo del AI
/// @param ninodo
/// @param inodo contenido a escribir
/// @return EXITO o FALLO
int escribir_inodo(unsigned int ninodo, union _inodo *inodo) {}

/// @brief leer el contenido de un determinado inodo del AI
/// @param ninodo
/// @param inodo variable donde volcar el contenido
/// @return EXITO o FALLO
int leer_inodo(unsigned int ninodo, union _inodo *inodo) {}

/// @brief reservar el primer inodo libre
/// @param tipo
/// @param permisos
/// @return posición del inodo reservado o FALLO
int reservar_inodo(unsigned char tipo, unsigned char permisos) {}