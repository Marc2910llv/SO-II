#include <string.h>
#include "ficheros_basico.h"

int tamMB(unsigned int nbloques)
{
    int restoMB = ((nbloques / 8) % BLOCKSIZE); // miramos el resto

    if (restoMB > 0) // en el caso de que mod no sea 0
    {
        return (((nbloques / 8) / BLOCKSIZE) + 1); // añadiremos 1 al resultado
    }
    else
    {
        return ((nbloques / 8) / BLOCKSIZE);
    }
}

int tamAI(unsigned int ninodos)
{
    int restoAI = ((ninodos * INODOSIZE) % BLOCKSIZE); // miramos el resto
    if (restoAI > 0)                                   // en el caso de que mod no sea 0
    {
        return ((ninodos * INODOSIZE) / BLOCKSIZE) + 1; // añadiremos 1 al resultado
    }
    else
    {
        return (ninodos * INODOSIZE) / BLOCKSIZE;
    }
}

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

    bwrite(posSB, &SB);
}

int initMB()
{ // Inicializa el mapa de bits
    // inicializamos mapa de bits a 0
    unsigned char *buf[BLOCKSIZE];
    memset(buf, 0, BLOCKSIZE);

    // leemos el superbloque
    struct superbloque SB;
    bread(0, &SB);
    // SB = punter;
    // int tamany;

    unsigned int pos = SB.posPrimerBloqueMB;
    unsigned int fin = SB.posUltimoBloqueMB;
    // tamany = tamMB(SB.totBloques);

    // Escribimos
    for (int i = pos; i < fin; i++)
    {
        bwrite(i, buf); // fer cridada al sistema
    }
}

int initAI()
{
    // SB.totinodos = ninodos;
    struct inodo inodos[BLOCKSIZE / INODOSIZE];
    struct superbloque SB; // revisar
    int j = bread(0, &SB);

    int contInodos = SB.cantInodosLibres + 1;                          // si hemos inicializado SB.posPrimerInodoLibre = 0
    for (int i = SB.posPrimerBloqueAI; i <= SB.posUltimoBloqueAI; i++) // para cada bloque del AI
    {
        for (int j = 0; j < BLOCKSIZE / INODOSIZE; j++) // para cada inodo del AI
        {
            inodos[j].tipo = 'l';          // libre
            if (contInodos < SB.totInodos) // si no hemos llegado al último inodo
            {
                inodos[j].punterosDirectos[0] = contInodos; // enlazamos con el siguiente
                contInodos++;
            }
            else // hemos llegado al último inodo
            {
                inodos[j].punterosDirectos[0] = 0; // NULL
                // hay que salir del bucle, el último bloque no tiene por qué estar completo !!!
            }
        }
        bwrite(i, inodos); // revisar, cridada al sistema
    }
}

int escribir_bit(unsigned int nbloque, unsigned int bit)
{
    struct superbloque SB;
    bread(0, &SB);

    int posbyte = nbloque / 8;
    int posbit = nbloque % 8;
    int nbloqueMB = posbyte / BLOCKSIZE;               // en qué bloque está el bit
    int nbloqueabs = SB.posPrimerBloqueMB + nbloqueMB; // pos absoluta del disp virtual en dnd se encuentra el bloque
    unsigned char bufferMB[BLOCKSIZE];
    posbyte = posbyte % BLOCKSIZE; // posició des byte dins del bloc
    unsigned char mascara = 128;   // 10000000
    mascara >>= posbit;            // desplazamiento de bits a la derecha

    if (bit == 1)
    {
        bufferMB[posbyte] |= mascara; //  operador OR para bits
    }
    else
    {
        bufferMB[posbyte] &= ~mascara; // operadores AND y NOT para bits
    }

    bwrite(nbloqueabs, bufferMB); // REVISAR
}

char leer_bit(unsigned int nbloque)
{
    struct superbloque SB;
    bread(0, &SB);

    int posbyte = nbloque / 8;
    int posbit = nbloque % 8;
    int nbloqueMB = posbyte / BLOCKSIZE;               // en qué bloque está el bit
    int nbloqueabs = SB.posPrimerBloqueMB + nbloqueMB; // pos absoluta del disp virtual en dnd se encuentra el bloque
    unsigned char bufferMB[BLOCKSIZE];
    posbyte = posbyte % BLOCKSIZE; // posició des byte dins del bloc
    unsigned char mascara = 128;   // 10000000
    mascara >>= posbit;            // desplazamiento de bits a la derecha
    mascara &= bufferMB[posbyte];  // operador AND para bits
    mascara >>= (7 - posbit);      // desplazamiento de bits a la derecha
    return mascara;
}

int reservar_bloque()
{
    struct superbloque SB;
    (bread(posSB, &SB);

    if (SB.cantBloquesLibres == 0)
    {
        perror("Error");
        return -1;
    }

    unsigned char bufferMB[BLOCKSIZE];
}

int escribir_inodo(unsigned int ninodo, struct inodo inodo)
{
    // leemos el superbloque
    struct superbloque SB;
    bread(0, &SB);

    int nBloqueArray = SB.posPrimerBloqueAI + (ninodo / (BLOCKSIZE / INODOSIZE)); // Obtenemos el nº de bloque del array de inodos que tiene el inodo solicitado
    struct inodo inodos[BLOCKSIZE / INODOSIZE];
    bread(nBloqueArray, &inodos); // Lo leemos de su posición absoluta del dispositivo

    int id = ninodo % (BLOCKSIZE / INODOSIZE);
    inodos[ninodo % (BLOCKSIZE / INODOSIZE)] = inodo; // escribimos el inodo en el lugar correspondiente del array

    int error = bwrite(nBloqueArray, &inodos); // El bloque modificado lo escribimos en el dispositivo virtual, devolvemos 0 si todo ha ido bien

    // inodi.mtime = time(NULL);

    if (error = -1)
    {
        return error;
    }
    else
    {
        return 0;
    }
}

int leer_inodo(unsigned int ninodo, struct inodo *inodo)
{
    // leemos el superbloque
    struct superbloque SB;
    bread(0, &SB);

    int nBloqueArray = ninodo / (BLOCKSIZE / INODOSIZE); // Obtenemos el nº de bloque del array de inodos que tiene el inodo solicitado
    struct inodo inodos[BLOCKSIZE / INODOSIZE];
    int error = bread(nBloqueArray, &inodos); // Lo leemos de su posición absoluta del dispositivo

    // inodi -> mtime = time(NULL);

    if (error = -1)
    {
        return error;
    }
    else
    {
        return 0;
    }
}

int reservar_inodo(unsigned char tipo, unsigned char permisos)
{
    struct inodo inodos[BLOCKSIZE / INODOSIZE];
    struct superbloque SB; // revisar
    int j = bread(0, &SB);

    int libres = 0;
    int posInodoReservado;
    for (int i = 0; i < inodos; i++)
    {
        if ((inodos[i].tipo == 'l') && (libres = 0))
        {
            posInodoReservado = i;
            libres = 1;
        }
    }

    if (libres == 0)
    {
        return -1;
    }

    SB.posPrimerInodoLibre = posInodoReservado + 1;
    inodos[posInodoReservado].tipo = tipo;
    inodos[posInodoReservado].permisos = permisos;
    inodos[posInodoReservado].nlinks = 1;
    inodos[posInodoReservado].tamEnBytesLog = 0;
    inodos[posInodoReservado]->atime = time(NULL);
    inodos[posInodoReservado]->mtime = time(NULL);
    inodos[posInodoReservado]->ctime = time(NULL);
    inodos[posInodoReservado].numBloquesOcupados = 0;
    inodos[posInodoReservado].punterosDirectos = 0;
    inodos[posInodoReservado].punterosIndirectos = 0;

    escribir_inodo(posInodoReservado, inodos);

    SB.cantInodosLibres = SB.cantBloquesLibres + 1;

    return posInodoReservado;
}