/*
 *   Marc Llobera Villalonga
 */

#include "bloques.h"
#include <time.h> //para los sellos de tiempo
#include <limits.h>

#define posSB 0 // el superbloque se escribe en el primer bloque de nuestro FS
#define tamSB 1

struct superbloque
{
    unsigned int posPrimerBloqueMB;                      // Posición absoluta del primer bloque del mapa de bits
    unsigned int posUltimoBloqueMB;                      // Posición absoluta del último bloque del mapa de bits
    unsigned int posPrimerBloqueAI;                      // Posición absoluta del primer bloque del array de inodos
    unsigned int posUltimoBloqueAI;                      // Posición absoluta del último bloque del array de inodos
    unsigned int posPrimerBloqueDatos;                   // Posición absoluta del primer bloque de datos
    unsigned int posUltimoBloqueDatos;                   // Posición absoluta del último bloque de datos
    unsigned int posInodoRaiz;                           // Posición del inodo del directorio raíz (relativa al AI)
    unsigned int posPrimerInodoLibre;                    // Posición del primer inodo libre (relativa al AI)
    unsigned int cantBloquesLibres;                      // Cantidad de bloques libres (en todo el disco)
    unsigned int cantInodosLibres;                       // Cantidad de inodos libres (en el AI)
    unsigned int totBloques;                             // Cantidad total de bloques del disco
    unsigned int totInodos;                              // Cantidad total de inodos (heurística)
    char padding[BLOCKSIZE - 12 * sizeof(unsigned int)]; // Relleno para ocupar el bloque completo
};

#define INODOSIZE 128 // tamaño en bytes de un inodo

/*typedef union _inodo
{
    struct
    {
        unsigned char tipo;     // Tipo ('l':libre, 'd':directorio o 'f':fichero)
        unsigned char permisos; // Permisos (lectura y/o escritura y/o ejecución)

        time_t atime; // Fecha y hora del último acceso a datos
        time_t mtime; // Fecha y hora de la última modificación de datos
        time_t ctime; // Fecha y hora de la última modificación del inodo

        unsigned int nlinks;             // Cantidad de enlaces de entradas en directorio
        unsigned int tamEnBytesLog;      // Tamaño en bytes lógicos (EOF)
        unsigned int numBloquesOcupados; // Cantidad de bloques ocupados zona de datos

        unsigned int punterosDirectos[12];  // 12 punteros a bloques directos
        unsigned int punterosIndirectos[3]; // 3 punteros a bloques indirectos:
        //1 indirecto simple, 1 indirecto doble, 1 indirecto triple
    };
    char padding[INODOSIZE];
} inodo_t;*/

struct inodo
{                           // comprobar que ocupa 128 bytes haciendo un sizeof(inodo)!!!
    unsigned char tipo;     // Tipo ('l':libre, 'd':directorio o 'f':fichero)
    unsigned char permisos; // Permisos (lectura y/o escritura y/o ejecución)

    /* Por cuestiones internas de alineación de estructuras, si se está utilizando
     un tamaño de palabra de 4 bytes (microprocesadores de 32 bits):
    unsigned char reservado_alineacion1 [2];
    en caso de que la palabra utilizada sea del tamaño de 8 bytes
    (microprocesadores de 64 bits): unsigned char reservado_alineacion1 [6]; */
    unsigned char reservado_alineacion1[6];

    time_t atime; // Fecha y hora del último acceso a datos
    time_t mtime; // Fecha y hora de la última modificación de datos
    time_t ctime; // Fecha y hora de la última modificación del inodo

    /* comprobar que el tamaño del tipo time_t para vuestra plataforma/compilador es 8:
    printf ("sizeof time_t is: %ld\n", sizeof(time_t)); */

    unsigned int nlinks;             // Cantidad de enlaces de entradas en directorio
    unsigned int tamEnBytesLog;      // Tamaño en bytes lógicos (EOF)
    unsigned int numBloquesOcupados; // Cantidad de bloques ocupados zona de datos

    unsigned int punterosDirectos[12];  // 12 punteros a bloques directos
    unsigned int punterosIndirectos[3]; /* 3 punteros a bloques indirectos:
    1 indirecto simple, 1 indirecto doble, 1 indirecto triple */

    /* Utilizar una variable de alineación si es necesario  para vuestra plataforma/compilador   */
    char padding[INODOSIZE - 2 * sizeof(unsigned char) - 3 * sizeof(time_t) - 18 * sizeof(unsigned int) - 6 * sizeof(unsigned char)];
    // Fijarse que también se resta lo que ocupen las variables de alineación utilizadas!!!
};

int tamMB(unsigned int nbloques);
int tamAI(unsigned int ninodos);
int initSB(unsigned int nbloques, unsigned int ninodos);
int initMB();
int initAI();