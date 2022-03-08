#include "ficheros_basico.h"

int tamMB(unsigned int nbloques)
{
    int restoMB = ((nbloques / 8) % BLOCKSIZE); //miramos el resto

    if (restoMB > 0) //en el caso de que mod no sea 0
    {
        return (((nbloques / 8) / BLOCKSIZE) + 1); //añadiremos 1 al resultado
    }
    else
    {
        return ((nbloques / 8) / BLOCKSIZE);
    }
}

int tamAI(unsigned int ninodos)
{
    int restoAI = ((ninodos * INODOSIZE) % BLOCKSIZE); //miramos el resto
    if (restoAI > 0) //en el caso de que mod no sea 0
    {
        return ((ninodos * INODOSIZE) / BLOCKSIZE) + 1; //añadiremos 1 al resultado
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

    bwrite(posSB, &SB); //fer cridada al sistema
}

int initMB() // Inicializa el mapa de bits
{
    //inicializamos mapa de bits a 0
    unsigned char buf[BLOCKSIZE]; 
    memset(buf, 0, BLOCKSIZE); 

    //leemos el superbloque
    struct superbloque SB;
    int pos = SB.posPrimerBloqueMB;
    int tam = SB.posUltimoBloqueMB-pos;

    //Escribimos
    for (int i = 0; i < tam; i++)
    {
        bwrite(pos++, buf); //fer cridada al sistema
    }
}

int initAI()
{
    //SB.totinodos = ninodos;
    struct inodo inodos[BLOCKSIZE / INODOSIZE];
    struct superbloque SB;
    int contInodos = SB.cantInodosLibres +1;                       // si hemos inicializado SB.posPrimerInodoLibre = 0
    for (int i = SB.posPrimerBloqueAI; i <= SB.posUltimoBloqueAI; i++) // para cada bloque del AI
    {
    //bread(¿?,&inodos[i]);
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
            inodos[j].punterosDirectos[0] = 0; //NULL
            // hay que salir del bucle, el último bloque no tiene por qué estar completo !!!
        }
    }
        bwrite(i,inodos); //revisar, cridada al sistema
    }
}