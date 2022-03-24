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

    bwrite(nbloqueabs, bufferMB);
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
    bread(posSB, &SB);

    if (SB.cantBloquesLibres == 0) // Comprobamos la variable del superbloque que nos indica si quedan bloques libres
    {
        perror("Error");
        return -1;
    }
    // si aún quedan, hemos de localizar el 1er bloque libre del dispositivo virtual consultando cuál es el primer bit a 0 en el MB

    // localizamos la posición en el dispositivo virtual, del primer bloque del MB que tenga algún bit a 0, nbloqueabs y lo leemos
    unsigned char bufferMB[BLOCKSIZE];
    unsigned char bufferAux[BLOCKSIZE];

    memset(bufferAux, 255, BLOCKSIZE); // llenamos el buffer auxiliar con 1s
    unsigned int nbloqueabs = SB.posPrimerBloqueMB;

    int iguales = 0;
    while (iguales == 0)
    {
        bread(nbloqueabs, bufferMB);                      // recorremos los bloques del MB (iterando con nbloqueabs) y los iremos cargando en bufferMB
        iguales = memcmp(bufferMB, bufferAux, BLOCKSIZE); // comparamos cada bloque leído del MB
        nbloqueabs++;
    }

    // localizamos qué byte dentro de ese bloque tiene algún 0
    int posbyte = 0;
    while (bufferMB[posbyte] == 255)
    {
        posbyte++;
    }

    // localizamos el primer bit dentro de ese byte que vale 0
    unsigned char mascara = 128; // 10000000
    int posbit = 0;
    // encontrar el primer bit a 0 en ese byte
    while (bufferMB[posbyte] & mascara)
    {                            // operador AND para bits
        bufferMB[posbyte] <<= 1; // desplazamiento de bits a la izquierda
        posbit++;
    }

    // determinar cuál es finalmente el nº de bloque físico(nbloque) que podemos reservar(posición absoluta del dispositivo)
    int nbloque = ((nbloqueabs - SB.posPrimerBloqueMB) * BLOCKSIZE + posbyte) * 8 + posbit;
    escribir_bit(nbloque, 1); // utilizamos la función escribit_bit() pasándole como parámetro ese nº de bloque y un 1 para indicar que el bloque está reservado
    SB.cantBloquesLibres--;
    bwrite(posSB, &SB); // decrementamos la cantidad de bloques libres en el campo correspondiente del superbloque, y salvamos el superbloque
    // limpiamos ese bloque en la zona de datos, grabando un buffer de 0s en la posición del nbloque del dispositivo,
    // por si había basura (podría tratarse de un bloque reutilizado por el sistema de ficheros)
    memset(bufferAux, 0, BLOCKSIZE);
    bwrite(SB.posPrimerBloqueDatos + nbloque - 1, bufferAux);

    return nbloque;
}

int liberar_bloque(unsigned int nbloque)
{
    struct superbloque SB;
    bread(posSB, &SB);

    // ponemos a 0 el bit del MB correspondiente al bloque nbloque
    escribir_bit(nbloque, 0);

    // incrementamos la cantidad de bloques libres en el superbloque
    SB.cantBloquesLibres++;
    bwrite(posSB, &SB);

    return nbloque;
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

    if (SB.cantBloquesLibres == 0)
    {
        return -1;
    }

    unsigned int posInodoReservado = SB.posPrimerInodoLibre;
    SB.posPrimerInodoLibre++;
    SB.cantInodosLibres--;

    struct inodo aux;
    aux.tipo = tipo;
    aux.permisos = permisos;
    aux.nlinks = 1;
    aux.tamEnBytesLog = 0;
    aux.atime = time(NULL);
    aux.mtime = time(NULL);
    aux.ctime = time(NULL);
    aux.numBloquesOcupados = 0;
    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            aux.punterosIndirectos[j] = 0;
        }
        aux.punterosDirectos[i] = 0;
    }

    escribir_inodo(posInodoReservado, inodos);

    bwrite(posSB, &SB);

    return posInodoReservado;
}

int obtener_nRangoBL(struct inodo *inodo, unsigned int nblogico, unsigned int *ptr)
{
    if (nblogico < DIRECTOS)
    {
        *ptr = inodo->punterosDirectos[nblogico];
        return 0;
    }
    else if (nblogico < INDIRECTOS0)
    {
        *ptr = inodo->punterosIndirectos[0];
        return 1;
    }
    else if (nblogico < INDIRECTOS1)
    {
        *ptr = inodo->punterosIndirectos[1];
        return 2;
    }
    else if (nblogico < INDIRECTOS2)
    {
        *ptr = inodo->punterosIndirectos[2];
        return 3;
    }
    else
    {
        *ptr = 0;
        error("Bloque lógico fuera de rango");
        return -1;
    }
}

int obtener_indice(unsigned int nblogico, int nivel_punteros)
{
    if (nblogico < DIRECTOS)
    {
        return nblogico;
    } // ej nblogico=8
    else if (nblogico < INDIRECTOS0)
    {
        return nblogico - DIRECTOS;
    } // ej nblogico=204
    else if (nblogico < INDIRECTOS1)
    { // ej nblogico=30.004
        if (nivel_punteros = 2)
        {
            return (nblogico - INDIRECTOS0) / NPUNTEROS;
        }
        else if (nivel_punteros = 1)
        {
            return (nblogico - INDIRECTOS0) % NPUNTEROS;
        }
    }
    else if (nblogico < INDIRECTOS2)
    { // ej nblogico=400.004
        if (nivel_punteros = 3)
        {
            return (nblogico - INDIRECTOS1) / (NPUNTEROS * NPUNTEROS);
        }
        else if (nivel_punteros = 2)
        {
            return ((nblogico - INDIRECTOS1) % (NPUNTEROS * NPUNTEROS)) / NPUNTEROS;
        }
        else if (nivel_punteros = 1)
        {
            return ((nblogico - INDIRECTOS1) % (NPUNTEROS * NPUNTEROS)) % NPUNTEROS;
        }
    }
}

int traducir_bloque_inodo(unsigned int ninodo, unsigned int nblogico, char reservar)
{
    struct inodo inodo[BLOCKSIZE / INODOSIZE];
    int ptr, ptr_ant, salvar_inodo, nRangoBL, nivel_punteros, indice;
    int buffer[NPUNTEROS];

    leer_inodo(ninodo, &inodo);
    ptr = 0, ptr_ant = 0, salvar_inodo = 0;
    nRangoBL = obtener_nRangoBL(&inodo, nblogico, &ptr); // 0:D, 1:I0, 2:I1, 3:I2
    nivel_punteros = nRangoBL;                           // el nivel_punteros +alto es el que cuelga del inodo
    while (nivel_punteros > 0)
    { // iterar para cada nivel de indirectos
        if (ptr = 0)
        { // no cuelgan bloques de punteros
            if (reservar = 0)
            {
                return -1;
            } // bloque inexistente -> no imprimir nada por pantalla!!!
            else
            { // reservar bloques punteros y crear enlaces desde inodo hasta datos
                salvar_inodo = 1;
                ptr = reservar_bloque(); // de punteros
                inodo.numBloquesOcupados++;
                inodo.ctime = time(NULL); // fecha actual
                if (nivel_punteros = nRangoBL)
                {                                                 // el bloque cuelga directamente del inodo
                    inodo.punterosIndirectos[nRangoBL - 1] = ptr; // (imprimirlo para test)
                }
                else
                {                         // el bloque cuelga de otro bloque de punteros
                    buffer[indice] = ptr; // (imprimirlo para test)
                    bwrite(ptr_ant, buffer);
                }
                memset(buffer, 0, BLOCKSIZE) // ponemos todos los punteros del buffer a 0
            }
        }
        else
        {
            bread(ptr, buffer);
        }
        indice = obtener_indice(nblogico, nivel_punteros);
        ptr_ant = ptr;        // guardamos el puntero
        ptr = buffer[indice]; // y lo desplazamos al siguiente nivel
        nivel_punteros--;
    } // al salir de este bucle ya estamos al nivel de datos
    if (ptr = 0)
    { // no existe bloque de datos
        if (reservar = 0)
        {
            return -1;
        } // error lectura ∄ bloque
        else
        {
            salvar_inodo = 1;
            ptr = reservar_bloque(); // de datos
            inodo.numBloquesOcupados++;
            inodo.ctime = time(NULL);
            if (nRangoBL = 0)
            {
                inodo.punterosDirectos[nblogico] = ptr; // (imprimirlo para test)
            }
            else
            {
                buffer[indice] = ptr; // (imprimirlo para test)
                bwrite(ptr_ant, buffer);
            }
        }
    }
    if (salvar_inodo = 1)
    {
        escribir_inodo(ninodo, inodo); // sólo si lo hemos actualizado
    }
    return ptr; // nbfisico del bloque de datos
}