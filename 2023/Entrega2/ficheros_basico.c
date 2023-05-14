/*
 *   Marc Llobera Villalonga
 */

#include "ficheros_basico.h"
#define DEBUG1 0 // leer_bit
#define DEBUG2 0 // traducir_bloque_inodo
#define DEBUG3 0 // liberar_bloques_inodo

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

    for (int i = SB.posPrimerBloqueMB; i <= SB.posUltimoBloqueMB; i++)
    {
        if (bwrite(i, bufferMB) == EXIT_FAILURE)
        {
            return EXIT_FAILURE;
        }
    }

    for (int i = posSB; i < SB.posPrimerBloqueDatos; i++)
    {
        reservar_bloque();
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

    int contInodos = SB.posPrimerInodoLibre + 1; // si hemos inicializado SB.posPrimerInodoLibre = 0
    int acabar = 0;
    for (int i = SB.posPrimerBloqueAI; (i <= SB.posUltimoBloqueAI) && acabar == 0; i++) // para cada bloque del AI
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
                acabar = 1;
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

    unsigned int posbyte = nbloque / 8;
    unsigned int posbit = nbloque % 8;

    unsigned int nbloqueMB = posbyte / BLOCKSIZE;

    unsigned int nbloqueabs = SB.posPrimerBloqueMB + nbloqueMB;

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

#if DEBUG1
    printf("[leer_bit(%i) → posbyte:%i, posbit:%i, nbloqueMB:%i, nbloqueabs:%i)]\n\n", nbloque, posbyte, posbit, nbloqueMB, nbloqueabs);
#endif

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
    int salir = 0;

    while (salir == 0) // No lee mas que una linea
    {
        if (bread(nbloqueabs, bufferMB) == EXIT_FAILURE)
        {
            fprintf(stderr, "Error while reading at reservar_bloque()\n");
            return EXIT_FAILURE;
        }

        if (memcmp(bufferMB, bufferAux, BLOCKSIZE) != 0)
        {
            salir = 1;
            break;
        }
        nbloqueabs++;
    }

    // Localizamos qué bytedentro de ese bloque tiene algún 0
    unsigned int posbyte = 0;
    while (bufferMB[posbyte] == 255)
    {
        posbyte++;
    }

    // Localizamos el primer bitdentro de ese byte que vale 0
    unsigned char mascara = 128; // 10000000
    unsigned int posbit = 0;
    // encontrar el primer bit a 0 en ese byte
    while (bufferMB[posbyte] & mascara)
    {                            // operadorAND para bits
        bufferMB[posbyte] <<= 1; // desplazamientode bits a la izquierda
        posbit++;
    }

    // Reservamos el bloque físico
    unsigned int nbloque = ((nbloqueabs - SB.posPrimerBloqueMB) * BLOCKSIZE + posbyte) * 8 + posbit;
    if (escribir_bit(nbloque, 1) == FALLO)
    {
        perror("Error reservar_bloque escribir_bit");
        return FALLO;
    }

    SB.cantBloquesLibres--;

    // Limpiamos el bloque de la zona de datos
    memset(bufferAux, 0, BLOCKSIZE);
    if (bwrite(SB.posPrimerBloqueDatos + nbloque - 1, bufferAux) == FALLO)
    {
        perror("Error reservar_bloque bwrite (limpiar bloque)");
        return FALLO;
    }

    if (bwrite(posSB, &SB) == FALLO)
    {
        perror("Error reservar_bloque bwrite (SB)");
        return FALLO;
    }

    return nbloque;
}

/// @brief liberar un bloque determinado
/// @param nbloque
/// @return nbloque liberado
int liberar_bloque(unsigned int nbloque)
{
    struct superbloque SB;
    if (bread(posSB, &SB) == FALLO)
    {
        perror("Error liberar_bloque bread");
        return FALLO;
    }

    if (escribir_bit(nbloque, 0) == FALLO)
    {
        perror("Error liberar_bloque escribir_bit");
        return FALLO;
    }

    SB.cantBloquesLibres++;

    if (bwrite(posSB, &SB) == FALLO)
    {
        perror("Error liberar_bloque bwrite");
        return FALLO;
    }

    return nbloque;
}

/// @brief escribir el contenido pasado por parámetro en un determinado inodo del AI
/// @param ninodo
/// @param inodo contenido a escribir
/// @return EXITO o FALLO
int escribir_inodo(unsigned int ninodo, union _inodo inodo)
{
    struct superbloque SB;
    if (bread(posSB, &SB) == FALLO)
    {
        perror("Error escribir_inodo bread (SB)");
        return FALLO;
    }

    // Obtenemos la posición absoluta del inodo
    unsigned int nbloque = SB.posPrimerBloqueAI + (ninodo / (BLOCKSIZE / INODOSIZE));

    union _inodo inodos[BLOCKSIZE / INODOSIZE];
    if (bread(nbloque, inodos) == FALLO)
    {
        perror("Error escribir_inodo bread (inodo)");
        return FALLO;
    }

    inodos[ninodo % (BLOCKSIZE / INODOSIZE)] = inodo;

    if (bwrite(nbloque, inodos) == FALLO)
    {
        perror("Error escribir_inodo bwrite (inodo)");
        return FALLO;
    }

    return EXITO;
}

/// @brief leer el contenido de un determinado inodo del AI
/// @param ninodo
/// @param inodo variable donde volcar el contenido
/// @return EXITO o FALLO
int leer_inodo(unsigned int ninodo, union _inodo *inodo)
{
    struct superbloque SB;
    if (bread(posSB, &SB) == FALLO)
    {
        perror("Error leer_inodo bread (SB)");
        return FALLO;
    }

    // Obtenemos la posición absoluta del inodo
    unsigned int nbloque = SB.posPrimerBloqueAI + (ninodo / (BLOCKSIZE / INODOSIZE));

    union _inodo inodos[BLOCKSIZE / INODOSIZE];
    if (bread(nbloque, inodos) == FALLO)
    {
        perror("Error leer_inodo bread (inodo)");
        return FALLO;
    }

    *inodo = inodos[ninodo % (BLOCKSIZE / INODOSIZE)];

    return EXITO;
}

/// @brief reservar el primer inodo libre
/// @param tipo
/// @param permisos
/// @return posición del inodo reservado -2 (no quedan inodos libres) o FALLO
int reservar_inodo(unsigned char tipo, unsigned char permisos)
{
    struct superbloque SB;
    if (bread(posSB, &SB) == FALLO)
    {
        perror("Error reservar_inodo bread");
        return FALLO;
    }

    if (SB.cantInodosLibres < 1)
    {
        return -2;
    }

    unsigned int posInodoReservado = SB.posPrimerInodoLibre;
    SB.posPrimerInodoLibre++;
    SB.cantInodosLibres--;

    union _inodo inodo;
    inodo.tipo = tipo;
    inodo.permisos = permisos;
    inodo.nlinks = 1;
    inodo.tamEnBytesLog = 0;
    inodo.atime = time(NULL);
    inodo.mtime = time(NULL);
    inodo.ctime = time(NULL);
    inodo.numBloquesOcupados = 0;
    // Poner a 0 los punteros
    for (int i = 0; i < 12; i++)
    {
        inodo.punterosDirectos[i] = 0;
    }
    for (int i = 0; i < 3; i++)
    {
        inodo.punterosIndirectos[i] = 0;
    }

    if (escribir_inodo(posInodoReservado, inodo) == FALLO)
    {
        perror("Error reservar_inodo escribir_inodo");
        return FALLO;
    }

    if (bwrite(posSB, &SB) == FALLO)
    {
        perror("Error reservar_inodo bwrite");
        return FALLO;
    }

    return posInodoReservado;
}

/// @brief obtener rango de punteros y dirección donde se situa el bloque lógico buscado
/// @param inodo
/// @param nblogico
/// @param ptr
/// @return rango de punteros o FALLO
int obtener_nRangoBL(union _inodo *inodo, unsigned int nblogico, unsigned int *ptr)
{
    if (nblogico < DIRECTOS)
    { // <12
        *ptr = inodo->punterosDirectos[nblogico];
        return 0;
    }
    else if (nblogico < INDIRECTOS0)
    { // <268
        *ptr = inodo->punterosIndirectos[0];
        return 1;
    }
    else if (nblogico < INDIRECTOS1)
    { // <65.804
        *ptr = inodo->punterosIndirectos[1];
        return 2;
    }
    else if (nblogico < INDIRECTOS2)
    { // <16.843.020
        *ptr = inodo->punterosIndirectos[2];
        return 3;
    }
    else
    {
        *ptr = 0;
        perror("Error obtener_nRangoBL 'Bloque lógico fuera de rango'");
        return FALLO;
    }
}

/// @brief obtener indice de los bloques de punteros
/// @param nblogico
/// @param nivel_punteros
/// @return indice de los bloques de punteros
int obtener_indice(unsigned int nblogico, int nivel_punteros)
{
    if (nblogico < DIRECTOS)
    {
        return nblogico;
    } // ej. nblogico=8
    else if (nblogico < INDIRECTOS0)
    {
        return nblogico - DIRECTOS;
    } // ej. nblogico=204
    else if (nblogico < INDIRECTOS1)
    { // ej. nblogico=30.004
        if (nivel_punteros == 2)
        {
            return (nblogico - INDIRECTOS0) / NPUNTEROS;
        }
        else if (nivel_punteros == 1)
        {
            return (nblogico - INDIRECTOS0) % NPUNTEROS;
        }
    }
    else if (nblogico < INDIRECTOS2)
    { // ej. nblogico=400.004
        if (nivel_punteros == 3)
        {
            return (nblogico - INDIRECTOS1) / (NPUNTEROS * NPUNTEROS);
        }
        else if (nivel_punteros == 2)
        {
            return ((nblogico - INDIRECTOS1) % (NPUNTEROS * NPUNTEROS)) / NPUNTEROS;
        }
        else if (nivel_punteros == 1)
        {
            return ((nblogico - INDIRECTOS1) % (NPUNTEROS * NPUNTEROS)) % NPUNTEROS;
        }
    }
    return EXITO;
}

/// @brief obtener el número de bloque físico correspondiente a un bloque lógico
/// @param ninodo
/// @param nblogico
/// @param reservar
/// @return número de bloque físico o FALLO
int traducir_bloque_inodo(unsigned int ninodo, unsigned int nblogico, char reservar)
{
    union _inodo inodo;

    if (leer_inodo(ninodo, &inodo) == FALLO)
    {
        perror("Error traducir_bloque_inodo leer_inodo");
        return FALLO;
    }

    unsigned int ptr = 0;
    unsigned int ptr_ant = 0;
    int escribirInodo = 0;
    int nRangoBL = obtener_nRangoBL(&inodo, nblogico, &ptr);
    int nivel_punteros = nRangoBL;
    int indice = 0;
    int buffer[NPUNTEROS];

    while (nivel_punteros > 0)
    {
        if (ptr == 0)
        {
            if (reservar == 0)
            {
                return FALLO;
            }
            else
            {
                escribirInodo = 1;
                ptr = reservar_bloque();
                inodo.numBloquesOcupados++;
                inodo.ctime = time(NULL);

                if (nivel_punteros == nRangoBL)
                {
                    inodo.punterosIndirectos[nRangoBL - 1] = ptr;
#if DEBUG2
                    printf("[traducir_bloque_inodo()→ inodo.punterosIndirectos[%i] = %i (reservado BF %i para punteros_nivel%i)]\n",
                           nRangoBL - 1, ptr, ptr, nivel_punteros);
#endif
                }
                else
                {
                    buffer[indice] = ptr;
#if DEBUG2
                    printf("[traducir_bloque_inodo()→ inodo.punteros_nivel%i[%i] = %i (reservado BF %i para punteros_nivel%i)]\n",
                           nivel_punteros, indice, ptr, ptr, nivel_punteros);
#endif
                    if (bwrite(ptr_ant, buffer) == FALLO)
                    {
                        perror("Error traducir_bloque_inodo bwrite");
                        return FALLO;
                    }
                }
            }
        }
        if (bread(ptr, buffer) == FALLO)
        {
            perror("Error traducir_bloque_inodo bread");
            return FALLO;
        }
        indice = obtener_indice(nblogico, nivel_punteros);
        ptr_ant = ptr;
        ptr = buffer[indice];
        nivel_punteros--;
    }

    if (ptr == 0)
    {
        if (reservar == 0)
        {
            return FALLO;
        }
        else
        {
            escribirInodo = 1;
            ptr = reservar_bloque();
            inodo.numBloquesOcupados++;
            inodo.ctime = time(NULL);

            if (nRangoBL == 0)
            {
                inodo.punterosDirectos[nblogico] = ptr;
#if DEBUG2
                printf("[traducir_bloque_inodo()→ inodo.punterosDirectos[%i] = %i (reservado BF %i para BL %i)]\n",
                       nblogico, ptr, ptr, nblogico);
#endif
            }
            else
            {
                buffer[indice] = ptr;
#if DEBUG2
                printf("[traducir_bloque_inodo()→ inodo.punteros_nivel1[%i] = %i (reservado BF %i para BL %i)]\n",
                       indice, ptr, ptr, nblogico);
#endif
                if (bwrite(ptr_ant, buffer) == FALLO)
                {
                    perror("Error traducir_bloque_inodo bwrite");
                    return FALLO;
                }
            }
        }
    }

    if (escribirInodo == 1)
    {
        if (escribir_inodo(ninodo, inodo) == FALLO)
        {
            perror("Error traducir_bloque_inodo escribir_inodo");
            return FALLO;
        }
    }

    return ptr;
}

/// @brief Liberar un inodo implica por un lado, que tal inodo pasará a la cabeza de la lista de inodos libres
/// @param ninodo
/// @return ninodo o FALLO
int liberar_inodo(unsigned int ninodo)
{
    union _inodo inodo;
    if (leer_inodo(ninodo, &inodo) == FALLO)
    {
        perror("Error liberar_inodo leer_inodo");
        return FALLO;
    }

    int bloquesLiberados = liberar_bloques_inodo(0, &inodo);
    if (bloquesLiberados == FALLO)
    {
        perror("Error liberar_inodo liberar_bloques_inodo");
        return FALLO;
    }
    inodo.numBloquesOcupados = inodo.numBloquesOcupados - bloquesLiberados;
    if (inodo.numBloquesOcupados != 0)
    {
        perror("Error liberar_inodo liberar_bloques_inodo");
        return FALLO;
    }

    inodo.tipo = 'l';
    inodo.tamEnBytesLog = 0;

    struct superbloque SB;
    if (bread(posSB, &SB) == FALLO)
    {
        perror("Error liberar_inodo bread");
        return FALLO;
    }

    int posAux = SB.posPrimerInodoLibre;
    SB.posPrimerInodoLibre = ninodo;
    inodo.punterosDirectos[0] = posAux;

    SB.cantInodosLibres++;

    inodo.ctime = time(NULL);

    if (escribir_inodo(ninodo, inodo) == FALLO)
    {
        perror("Error liberar_inodo escribir_inodo");
        return FALLO;
    }

    if (bwrite(posSB, &SB) == FALLO)
    {
        perror("Error liberar_inodo bwrite");
        return FALLO;
    }

    return ninodo;
}

/// SE PUEDE IMPLEMENTAR MEJORA
/// @brief libera todos los bloques ocupados a partir del bloque lógico indicado por el argumento
/// @param primerBL
/// @param inodo
/// @return bloques liberados, 0 (fichero vacío) o FALLO
int liberar_bloques_inodo(unsigned int primerBL, union _inodo *inodo)
{
    // libera los bloques de datos e índices iterando desde el primer bloque lógico a liberar hasta el
    // último por tanto explora las ramificaciones de punteros desde las hojas hacia las raíces en el inodo

    unsigned int nivel_punteros, indice, ptr, nBL, ultimoBL;
    int nRangoBL;
    unsigned int bloques_punteros[3][NPUNTEROS]; // array de bloques de punteros
    unsigned char bufAux_punteros[BLOCKSIZE];
    int ptr_nivel[3];  // punteros a bloques de punteros de cada nivel
    int indices[3];    // indices de cada nivel
    int liberados = 0; // nº de bloques liberados
    int breads = 0;
    int bwrites = 0;

    liberados = 0;
    if (inodo->tamEnBytesLog == 0)
    {
        return 0;
    } // el fichero está vacío
    // obtenemos el último bloque lógico del inodo
    if (inodo->tamEnBytesLog % BLOCKSIZE == 0)
    {
        ultimoBL = ((inodo->tamEnBytesLog) / BLOCKSIZE) - 1;
    }
    else
    {
        ultimoBL = (inodo->tamEnBytesLog) / BLOCKSIZE;
    }
    memset(bufAux_punteros, 0, BLOCKSIZE);
    ptr = 0;
#if DEBUG3
    printf("[liberar_bloques_inodo()-> primerBL: %d, ultimoBL: %d]\n", primerBL, ultimoBL);
#endif
    for (nBL = primerBL; nBL <= ultimoBL; nBL++)
    {                                                  // recorrido BLs
        nRangoBL = obtener_nRangoBL(inodo, nBL, &ptr); // 0:D, 1:I0, 2:I1, 3:I2
        if (nRangoBL < 0)
        {
            perror("Error liberar_bloques_inodo obtener_nRangoBL");
            return FALLO;
        }
        nivel_punteros = nRangoBL; // el nivel_punteros +alto cuelga del inodo

        while (ptr > 0 && nivel_punteros > 0)
        { // cuelgan bloques de punteros
            indice = obtener_indice(nBL, nivel_punteros);
            if (indice == 0 || nBL == primerBL)
            {
                // solo hay que leer del dispositivo si no está ya cargado previamente en un buffer
                if (bread(ptr, bloques_punteros[nivel_punteros - 1]) == FALLO)
                {
                    perror("Error liberar_bloques_inodo bread");
                    return FALLO;
                }
                breads++;
            }
            ptr_nivel[nivel_punteros - 1] = ptr;
            indices[nivel_punteros - 1] = indice;
            ptr = bloques_punteros[nivel_punteros - 1][indice];
            nivel_punteros--;
        }

        if (ptr > 0)
        { // si existe bloque de datos
            if (liberar_bloque(ptr) == FALLO)
            {
                perror("Error liberar_bloques_inodo liberar_bloque");
                return FALLO;
            }
            liberados++;
#if DEBUG3
            printf("[liberar_bloques_inodo()-> liberado BF %d de datos par a BL %d]\n", ptr, nBL);
#endif

            if (nRangoBL == 0)
            { // es un puntero Directo
                inodo->punterosDirectos[nBL] = 0;
            }
            else
            {
                nivel_punteros = 1;
                while (nivel_punteros <= nRangoBL)
                {
                    indice = indices[nivel_punteros - 1];
                    bloques_punteros[nivel_punteros - 1][indice] = 0;
                    ptr = ptr_nivel[nivel_punteros - 1];
                    if (memcmp(bloques_punteros[nivel_punteros - 1], bufAux_punteros, BLOCKSIZE) == 0)
                    {
                        // No cuelgan más bloques ocupados, hay que liberar el bloque de punteros
                        if (liberar_bloque(ptr) == FALLO)
                        {
                            perror("Error liberar_bloques_inodo liberar_bloque");
                            return FALLO;
                        }
                        liberados++;

#if DEBUG3
                        printf("[liberar_bloques_inodo()-> liberado BF %i de punteros_nivel%i correspondiente al BL: %i]\n", ptr, nivel_punteros, nBL);
#endif

                        //  MEJORA

                        //

                        if (nivel_punteros == nRangoBL)
                        {
                            inodo->punterosIndirectos[nRangoBL - 1] = 0;
                        }
                        nivel_punteros++;
                    }
                    else
                    { // escribimos en el dispositivo el bloque de punteros modificado
                        if (bwrite(ptr, bloques_punteros[nivel_punteros - 1]) == FALLO)
                        {
                            perror("Error liberar_bloques_inodo bwrite");
                            return FALLO;
                        }
                        bwrites++;
                        // hemos de salir del bucle ya que no será necesario liberar los bloques de niveles
                        // superiores de los que cuelga
                        nivel_punteros = nRangoBL + 1;
                    }
                }
            }
        }
        else
        {
            // MEJORA

            //
        }
    }

#if DEBUG3
    printf("[liberar_bloques_inodo()-> total bloques liberados: %d, total breads: %d, total bwrites: %d]\n", liberados, breads, bwrites);
#endif

    return liberados;
}