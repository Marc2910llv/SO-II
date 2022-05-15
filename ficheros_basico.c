//Pere Joan Vives Morey
//Marc Llobera Villalonga
//Carlos Lozano Alemañy
#include <string.h>
#include "ficheros_basico.h"
#include <time.h>

int tamMB(unsigned int nbloques)
{
    int restoMB = ((nbloques / 8) % BLOCKSIZE); // miramos el resto
    if (restoMB > 0)                            // en el caso de que mod no sea 0
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

int initSB(unsigned int nbloques, unsigned int ninodos) // Inicializa los datos del superbloque
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

    if (bwrite(posSB, &SB) == -1)
    {
        perror("ERROR EN initSB");
        return -1;
    }
    return 0;
}

int initMB() // Inicializa el mapa de bits
{
    // inicializamos mapa de bits a 0
    unsigned char buf[BLOCKSIZE];
    memset(buf, 0, BLOCKSIZE);

    struct superbloque SB;
    if (bread(posSB, &SB) == -1)
    {
        perror("ERROR EN initMB AL LEER EL SUPERBLOQUE");
    }

    int inicio = SB.posPrimerBloqueMB;
    int tamMB = SB.posUltimoBloqueMB - SB.posPrimerBloqueMB;
    // tamany = tamMB(SB.totBloques);

    // Escribimos
    for (int i = inicio; i <= tamMB + inicio; i++)
    {
        if (bwrite(i, buf) == -1)
        {
            perror("ERROR EN initMB");
        }
    }

    for (int i = posSB; i < SB.posPrimerBloqueDatos; i++)
    {
        reservar_bloque();
    }

    return 0;
}

int initAI() // Inicializar la lista de inodos libres
{
    // SB.totinodos = ninodos;
    struct inodo inodos[BLOCKSIZE / INODOSIZE];
    struct superbloque SB; // revisar
    if (bread(posSB, &SB) == -1)
    {
        perror("ERROR EN initMB AL LEER EL SUPERBLOQUE");
    }

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
                break;
                // hay que salir del bucle, el último bloque no tiene por qué estar completo !!!
            }
        }
        if (bwrite(i, &inodos) == -1)
        {
            perror("ERROR EN initMB AL ESCRIBIR ALGUN INODO");
            return -1;
        }
    }
    return 0;
}

// Esta función escribe el valor indicado por el parámetro bit del MB que representa el bloque nbloque
int escribir_bit(unsigned int nbloque, unsigned int bit)
{
    struct superbloque SB;
    if (bread(posSB, &SB) == -1)
    {
        perror("ERROR EN escribir_bit AL LEER EL SUPERBLOQUE");
        return -1;
    }

    int posbyte = nbloque / 8;
    int posbit = nbloque % 8;
    int nbloqueMB = posbyte / BLOCKSIZE;               // en qué bloque está el bit
    int nbloqueabs = SB.posPrimerBloqueMB + nbloqueMB; // pos absoluta del disp virtual en dnd se encuentra el bloque
    unsigned char bufferMB[BLOCKSIZE];
    if (bread(nbloqueabs, bufferMB) == -1)
    {
        perror("ERROR EN escribir_bit AL LEER EL BLOQUE FÍSICO QUE CONTIENE EL BIT QUE HEMOS UBICADO");
        return -1;
    }
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

    if (bwrite(nbloqueabs, bufferMB) == -1)
    {
        perror("ERROR EN escribir_bit AL ESCRIBIR EL BUFFER DEL MB EN EL DISPOSITIVO VIRTUAL");
        return -1;
    }
    return 0;
}

char leer_bit(unsigned int nbloque) // Lee un determinado bit del MB y devuelve el valor del bit leído
{
    struct superbloque SB;
    if (bread(posSB, &SB) == -1)
    {
        perror("ERROR EN leer_bit AL LEER EL SUPERBLOQUE");
    }

    int posbyte = nbloque / 8;
    int posbit = nbloque % 8;
    int nbloqueMB = posbyte / BLOCKSIZE;               // en qué bloque está el bit
    int nbloqueabs = SB.posPrimerBloqueMB + nbloqueMB; // pos absoluta del disp virtual en dnd se encuentra el bloque
    unsigned char bufferMB[BLOCKSIZE];
    if (bread(nbloqueabs, bufferMB) == -1)
    {
        perror("ERROR EN leer_bit AL LEER EL BLOQUE FÍSICO QUE CONTIENE EL BIT QUE HEMOS UBICADO");
    }
    posbyte = posbyte % BLOCKSIZE; // posició des byte dins del bloc
    unsigned char mascara = 128;   // 10000000
    mascara >>= posbit;            // desplazamiento de bits a la derecha
    mascara &= bufferMB[posbyte];  // operador AND para bits
    mascara >>= (7 - posbit);      // desplazamiento de bits a la derecha

    return mascara;
}

// Encuentra el primer bloque libre, consultando el MB, lo ocupa y devuelve su posición
int reservar_bloque()
{
    struct superbloque SB;
    if (bread(posSB, &SB) == -1)
    {
        perror("ERROR EN reservar_bloque AL LEER EL SUPERBLOQUE");
    }

    if (SB.cantBloquesLibres == 0) // Comprobamos la variable del superbloque que nos indica si quedan bloques libres
    {
        perror("ERROR EN reservar_bloque, NO QUEDAN BLOQUES LIBRES");
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
        if (bread(nbloqueabs, bufferMB) == -1) // recorremos los bloques del MB (iterando con nbloqueabs) y los iremos cargando en bufferMB
        {
            perror("ERROR EN reservar_bloque AL LEER");
        }
        iguales = memcmp(bufferMB, bufferAux, BLOCKSIZE); // comparamos cada bloque leído del MB
        if (iguales != 0)
        {
            break;
        }
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
    if (bwrite(posSB, &SB) == -1)
    {
        perror("ERROR EN reservar_bloque AL SALVAR EL SUPERBLOQUE");
    }
    // decrementamos la cantidad de bloques libres en el campo correspondiente del superbloque, y salvamos el superbloque
    // limpiamos ese bloque en la zona de datos, grabando un buffer de 0s en la posición del nbloque del dispositivo,
    // por si había basura (podría tratarse de un bloque reutilizado por el sistema de ficheros)
    memset(bufferAux, 0, BLOCKSIZE);
    if (bwrite(SB.posPrimerBloqueDatos + nbloque - 1, bufferAux) == -1)
    {
        perror("ERROR EN reservar_bloque AL GRABAR EL BUFFER DE 0s EN LA POSICIÓN NBLOQUE");
    }

    return nbloque;
}

int liberar_bloque(unsigned int nbloque) // Libera un bloque determinado
{
    struct superbloque SB;
    if (bread(posSB, &SB) == -1)
    {
        perror("ERROR EN liberar_bloque AL LEER EL SUPERBLOQUE");
        return -1;
    }

    // ponemos a 0 el bit del MB correspondiente al bloque nbloque
    escribir_bit(nbloque, 0);

    // incrementamos la cantidad de bloques libres en el superbloque
    SB.cantBloquesLibres++;
    if (bwrite(posSB, &SB) == -1)
    {
        perror("ERROR EN liberar_bloque AL ESCRIBIR INCREMENTANDO LA CANTIDAD DE BLOQUES LIBRES EL SUPERBLOQUE");
        return -1;
    }

    return nbloque;
}

// Escribe el contenido de una variable de tipo struct inodo en un determinado inodo del array de inodos, inodos
int escribir_inodo(unsigned int ninodo, struct inodo inodo)
{
    // leemos el superbloque
    struct superbloque SB;
    if (bread(posSB, &SB) == -1)
    {
        perror("ERROR EN escribir_inodo AL LEER EL SUPERBLOQUE");
        return -1;
    }

    int nBloqueArray = SB.posPrimerBloqueAI + (ninodo / (BLOCKSIZE / INODOSIZE)); // Obtenemos el nº de bloque del array de inodos que tiene el inodo solicitado
    struct inodo inodos[BLOCKSIZE / INODOSIZE];
    if (bread(nBloqueArray, inodos) == -1) // Lo leemos de su posición absoluta del dispositivo
    {
        perror("ERROR EN escribir_inodo AL LEER EL INODO");
        return -1;
    }
    inodos[ninodo % (BLOCKSIZE / INODOSIZE)] = inodo; // escribimos el inodo en el lugar correspondiente del array

    // inodi.mtime = time(NULL);

    if (bwrite(nBloqueArray, inodos) == -1) // El bloque modificado lo escribimos en el dispositivo virtual, devolvemos 0 si todo ha ido bien
    {
        perror("ERROR EN escribir_inodo AL ESCRIBIR EL BLOQUE MODIFICADO EN EL DISPOSITIVO VIRTUAL");
        return -1;
    }
    else
    {
        return 0;
    }
}

// Lee un determinado inodo del array de inodos para volcarlo en una variable de tipo struct inodo pasada por referencia
int leer_inodo(unsigned int ninodo, struct inodo *inodo)
{
    // leemos el superbloque
    struct superbloque SB;
    if (bread(posSB, &SB) == -1)
    {
        perror("ERROR EN leer_inodo AL LEER EL SUPERBLOQUE");
        return -1;
    }

    int nBloqueArray = SB.posPrimerBloqueAI + (ninodo / (BLOCKSIZE / INODOSIZE)); // Obtenemos el nº de bloque del array de inodos que tiene el inodo solicitado
    struct inodo inodos[BLOCKSIZE / INODOSIZE];
    if (bread(nBloqueArray, inodos) == -1) // Lo leemos de su posición absoluta del dispositivo
    {
        perror("ERROR EN leer_inodo AL LEER EL INODO SOLICITADO");
        return -1;
    }

    // inodi -> mtime = time(NULL);
    *inodo = inodos[ninodo % (BLOCKSIZE / INODOSIZE)]; // inodo que volem dins l'array d'inodos que hem llegit
    return 0;
}

// Encuentra el primer inodo libre, lo reserva, devuelve su número y actualiza la lista enlazada de inodos libres
int reservar_inodo(unsigned char tipo, unsigned char permisos)
{
    struct superbloque SB;
    if (bread(posSB, &SB) == -1)
    {
        perror("ERROR EN reservar_inodo AL LEER EL SUPERBLOQUE");
        return -1;
    }

    if (SB.cantInodosLibres == 0)
    {
        perror("ERROR EN reservar_inodo, NO QUEDAN BLOQUES LIBRES");
        return -1;
    }

    unsigned int posInodoReservado = SB.posPrimerInodoLibre;
    SB.posPrimerInodoLibre++;
    SB.cantInodosLibres--;

    struct inodo nuevo;
    nuevo.tipo = tipo;
    nuevo.permisos = permisos;
    nuevo.nlinks = 1;
    nuevo.tamEnBytesLog = 0;
    nuevo.atime = time(NULL);
    nuevo.mtime = time(NULL);
    nuevo.ctime = time(NULL);
    nuevo.numBloquesOcupados = 0;

    // Poner a 0 los punteros
    for (int i = 0; i < 12; i++)
    {
        nuevo.punterosDirectos[i] = 0;
    }
    for (int j = 0; j < 3; j++)
    {
        nuevo.punterosIndirectos[j] = 0;
    }
    escribir_inodo(posInodoReservado, nuevo);
    //SB.cantInodosLibres = SB.cantInodosLibres - 1;
    if (bwrite(posSB, &SB) == -1)
    {
        perror("ERROR EN reservar_inodo AL REESCRIBIR EL SUPERBLOQUE");
        return -1;
    }

    return posInodoReservado;
}

// Obtener el rango de punteros en el que se sitúa el bloque lógico que buscamos
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
        perror("ERROR EN obtener_nRangoBL, BLOQUE LÓGICO FUERA DE RANGO");
        return -1;
    }
}

// Generalizar la obtención de los índices de los bloques de punteros
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
    { // ej nblogico=400.004
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
    return 0;
}

// Esta función se encarga de obtener el nº de bloque físico correspondiente a un bloque lógico determinado del inodo indicado
int traducir_bloque_inodo(unsigned int ninodo, unsigned int nblogico, char reservar)
{
    struct inodo inodo;
    int ptr_ant, salvar_inodo, nRangoBL, nivel_punteros, indice;
    unsigned int ptr;
    int buffer[NPUNTEROS];

    leer_inodo(ninodo, &inodo);
    ptr = 0, ptr_ant = 0, salvar_inodo = 0;
    nRangoBL = obtener_nRangoBL(&inodo, nblogico, &ptr); // 0:D, 1:I0, 2:I1, 3:I2
    nivel_punteros = nRangoBL;                           // el nivel_punteros +alto es el que cuelga del inodo
    while (nivel_punteros > 0)
    { // iterar para cada nivel de indirectos
        if (ptr == 0)
        { // no cuelgan bloques de punteros
            if (reservar == 0)
            {
                return -1;
            } // bloque inexistente -> no imprimir nada por pantalla!!!
            else
            { // reservar bloques punteros y crear enlaces desde inodo hasta datos
                salvar_inodo = 1;
                ptr = reservar_bloque(); // de punteros
                inodo.numBloquesOcupados++;
                inodo.ctime = time(NULL); // fecha actual
                if (nivel_punteros == nRangoBL)
                {                                                 // el bloque cuelga directamente del inodo
                    inodo.punterosIndirectos[nRangoBL - 1] = ptr; // (imprimirlo para test)
                    //printf("traducir_bloque_inodo()→ inodo.punterosIndirectos[%i] = %i (reservado BF %i para BL %i)\n", nRangoBL-1, ptr, ptr, nivel_punteros);
                }
                else
                {                         // el bloque cuelga de otro bloque de punteros
                    buffer[indice] = ptr; // (imprimirlo para test)
                    //printf("traducir_bloque_inodo()→ inodo.punteros_nivel %i[%i] = %i (reservado BF %i para BL %i)\n", nivel_punteros+1,indice, ptr, ptr, nivel_punteros);
                    bwrite(ptr_ant, buffer);
                }
                memset(buffer, 0, BLOCKSIZE); // ponemos todos los punteros del buffer a 0
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
    if (ptr == 0)
    { // no existe bloque de datos
        if (reservar == 0)
        {
            return -1;
        } // error lectura ∄ bloque
        else
        {
            salvar_inodo = 1;
            ptr = reservar_bloque(); // de datos
            inodo.numBloquesOcupados++;
            inodo.ctime = time(NULL);
            if (nRangoBL == 0)
            {
                inodo.punterosDirectos[nblogico] = ptr; // (imprimirlo para test)
                //printf("traducir_bloque_inodo()→ inodo.punterosDirectos[%i] = %i (reservado BF %i para BL %i)\n", nblogico, ptr, ptr, nblogico);
            }
            else
            {
                buffer[indice] = ptr; // (imprimirlo para test)
                //printf("traducir_bloque_inodo()→ inodo.punteros_nivel %i[%i] = %i (reservado BF %i para punteros_nivel%i)\n",nivel_punteros+1, indice, ptr, ptr, nblogico);
                bwrite(ptr_ant, buffer);
            }
        }
    }
    if (salvar_inodo == 1)
    {
        escribir_inodo(ninodo, inodo); // sólo si lo hemos actualizado
    }
    return ptr; // nbfisico del bloque de datos
}

// Liberar un inodo implica por un lado, que tal inodo pasará a la cabeza de la lista de inodos libres y tendremos un inodo más libre en el sistema
int liberar_inodo(unsigned int ninodo)
{
    struct inodo inodo;
    if (leer_inodo(ninodo, &inodo) == -1)
    {
        perror("ERROR EN liberar_inodo AL LEER EL INODO");
        return -1;
    }

    int bloquesLiberados = liberar_bloques_inodo(0, &inodo);
    if (bloquesLiberados == -1)
    {
        perror("ERROR EN liberar_inodo AL LIBERAR LOS BLOQUES OCUPADOS");
        return -1;
    }

    inodo.numBloquesOcupados = inodo.numBloquesOcupados - bloquesLiberados;

    inodo.tipo = 'l';
    inodo.tamEnBytesLog = 0;

    /*if (escribir_inodo(ninodo, inodo) == -1)
    {
        perror("ERROR EN liberar_inodo AL ACTUALIZAR EL INODO");
        return -1;
    }*/

    struct superbloque SB;
    if (bread(posSB, &SB) == -1)
    {
        perror("ERROR EN liberar_inodo AL LEER EL SUPERBLOQUE");
        return -1;
    }

    int aux = SB.posPrimerInodoLibre;
    SB.posPrimerInodoLibre = ninodo;
    inodo.punterosDirectos[0] = aux;

    /*if (escribir_inodo(SB.posPrimerInodoLibre, inodo) == -1)
    {
        perror("ERROR EN liberar_inodo AL ACTUALIZAR EL SUPERBLOQUE CON EL NUEVO INODO LIBRE");
        return -1;
    }*/

    SB.cantInodosLibres = SB.cantInodosLibres + 1;

    if (escribir_inodo(ninodo, inodo) == -1)
    {
        perror("ERROR EN liberar_inodo AL ESCRIBIR EL INODO");
        return -1;
    }

    if (bwrite(posSB, &SB) == -1)
    {
        perror("ERROR EN liberar_inodo AL REESCRIBIR EL SUPERBLOQUE");
        return -1;
    }

    return ninodo;
}

// Libera todos los bloques ocupados
int liberar_bloques_inodo(unsigned int primerBL, struct inodo *inodo)
{
    // libera los bloques de datos e índices iterando desde el primer bloque lógico a liberar hasta el último
    // por tanto explora las ramificaciones de punteros desde las hojas hacia las raíces en el inodo

    unsigned int nivel_punteros, indice, ptr, nBL, ultimoBL;
    int nRangoBL;
    unsigned int bloques_punteros[3][NPUNTEROS]; // array de bloques de punteros
    unsigned char bufAux_punteros[BLOCKSIZE];
    int ptr_nivel[3];  // punteros a bloques de punteros de cada nivel
    int indices[3];    // indices de cada nivel
    int liberados = 0; // nº de bloques liberados
    int breads =0;
    int bwrites =0;

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

    //printf("[liberar_bloques_inodo()-> primerBL: %d, ultimoBL: %d]\n", primerBL, ultimoBL);

    for (nBL = primerBL; nBL <= ultimoBL; nBL++)
    {                                                  // recorrido BLs
        nRangoBL = obtener_nRangoBL(inodo, nBL, &ptr); // 0:D, 1:I0, 2:I1, 3:I2
        if (nRangoBL < 0)
        {
            perror("ERROR EN liberar_bloques_inodo nRangoBL es negativo");
            return -1;
        }
        nivel_punteros = nRangoBL; // el nivel_punteros +alto cuelga del inodo

        while (ptr > 0 && nivel_punteros > 0)
        { // cuelgan bloques de punteros
            indice = obtener_indice(nBL, nivel_punteros);
            if (indice == 0 || nBL == primerBL)
            {
                // solo hay que leer del dispositivo si no está ya cargado previamente en un buffer
                if (bread(ptr, bloques_punteros[nivel_punteros - 1]) == -1)
                {
                    perror("ERROR EN liberar_bloques_inodo AL LEER EL DISPOSITIVO");
                    return -1;
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
            if (liberar_bloque(ptr) == -1)
            {
                perror("ERROR EN liberar_bloques_inodo AL INTENRAR LIBERAR EL BLOQUE DE POSICIÓN ptr");
                return -1;
            }
            liberados++;

            //printf("[liberar_bloques_inodo()-> liberado BF %d de datos par a BL %d]\n", ptr, nBL);
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
                        if (liberar_bloque(ptr) == -1)
                        {
                            perror("ERROR EN liberar_bloques_inodo AL INTENRAR LIBERAR EL BLOQUE DE POSICIÓN ptr");
                            return -1;
                        }
                        liberados++;
                        //printf("[liberar_bloques_inodo()→ liberado BF %i de punteros_nivel%i correspondiente al BL: %i]\n", ptr, nivel_punteros, nBL);
                        // Incluir mejora para saltar los bloques que no sea necesario explorar !!!
                        //...
                        if (nivel_punteros == nRangoBL)
                        {
                            inodo->punterosIndirectos[nRangoBL - 1] = 0;
                        }
                        nivel_punteros++;
                    }
                    else
                    { // escribimos en el dispositivo el bloque de punteros modificado
                        if (bwrite(ptr, bloques_punteros[nivel_punteros - 1]) == -1)
                        {
                            perror("ERROR EN liberar_bloques_inodo AL ESCRIBIR EN EL DISPOSITIVO EL BLOQUE DE PUNTEROS MODIFICADO");
                            return -1;
                        }
                        bwrites++;
                        // hemos de salir del bucle ya que no será necesario liberar los bloques de niveles
                        // superiores de los que cuelga
                        nivel_punteros = nRangoBL + 1;
                    }
                }
            }
        }
    }
    //printf("[liberar_bloques_inodo()-> total bloques liberados: %d, total breads: %d, total bwrites: %d]\n", liberados,breads,bwrites);
    return liberados;
}