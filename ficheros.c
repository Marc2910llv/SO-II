#include "ficheros.h"

// Escribe el contenido procedente de un buffer de memoria, buf_original, de tamaño nbytes, en un fichero/directorio.
int mi_write_f(unsigned int ninodo, const void *buf_original, unsigned int offset, unsigned int nbytes)
{
    unsigned char *buf_bloque[BLOCKSIZE];
    int primerBL, ultimoBL, desp1, desp2, nbfisico;
    size_t bytesEscritos;

    struct inodo inodo;
    leer_inodo(ninodo, &inodo);

    if ((inodo.permisos & 2) != 2) // Comprobamos si tenemos permisos de escritura
    {
        perror("NO HAY PERMISO DE ESCRITURA") return -1;
    }

    primerBL = offset / BLOCKSIZE;
    ultimoBL = (offset + nbytes - 1) / BLOCKSIZE;
    desp1 = offset % BLOCKSIZE;
    desp2 = (offset + nbytes - 1) % BLOCKSIZE;

    // CASO 1º (el buffer cabe en un solo bloque)
    if (primerBL == ultimoBL)
    {
        nbfisico = traducir_bloque_inodo(ninodo, primerBL, 1); // Obtenemos el nº de bloque físico
        bread(nbfisico, buf_bloque);
        memcpy(buf_bloque + desp1, buf_original, nbytes); // Utilizamos memcpy() para escribir los nbytes
        bwrite(nbfisico, buf_bloque);

        bytesEscritos = nbytes;
    }
    // CASO 2º (la operación de escritura ocupa más de un bloque)
    else if (primerBL < ultimoBL)
    {
        // 1) Primer bloque lógico (BL8)
        nbfisico = traducir_bloque_inodo(ninodo, primerBL, 1); // Obtenemos el nº de bloque físico
        bread(nbfisico, buf_bloque);
        memcpy(buf_bloque + desp1, buf_original, BLOCKSIZE - desp1);
        bwrite(nbfisico, buf_bloque);

        // Bytes escritos habiendo corrido desp1 y estando situados ya en el primer bloque
        bytesEscritos = bytesEscritos + BLOCKSIZE - desp1;

        // 2) Bloques lógicos intermedios (bloques 9, 10, 11)
        for (int i = primerBL + 1; i < ultimoBL; i++)
        {
            // Obtenemos cada bloque intermedio
            nbfisico = traducir_bloque_inodo(ninodo, i, 1);
            bwrite(nbfisico, buf_original + (BLOCKSIZE - desp1) + (i - primerBL - 1) * BLOCKSIZE);

            bytesEscritos = bytesEscritos + BLOCKSIZE;
        }

        // 3) Último bloque lógico (bloque 12)
        nbfisico = traducir_bloque_inodo(ninodo, ultimoBL, 1);
        bread(nbfisico, buf_bloque);
        memcpy(buf_bloque, buf_original + (nbytes - desp2 - 1), desp2 + 1);
        bwrite(nbfisico, buf_bloque);

        bytesEscritos = bytesEscritos + desp2 + 1;
    }

    // ACTUALIZAMOS METAINFORMACIÓN DEL INODO
    leer_inodo(ninodo, &inodo);
    // Actualizar el tamaño en bytes lógico (si hemos escrito más allá del final del fichero)
    if (inodo.tamEnBytesLog < (nbytes + offset))
    {
        inodo.tamEnBytesLog = nbytes + offset;
        inodo.ctime = time(NULL);
    }

    inodo.mtime = time(NULL);

    escribir_inodo(ninodo, inodo);

    return bytesEscritos;
}

// Lee información de un fichero/directorio y la almacena en un buffer de memoria, buf_original
int mi_read_f(unsigned int ninodo, void *buf_original, unsigned int offset, unsigned int nbytes) //////////////REVISAR//////////////////
{
    // VARIABLES
    unsigned char *buf_bloque[BLOCKSIZE];
    int primerBL, ultimoBL, desp1, desp2, nbfisico, bytesLeidos;
    struct inodo inodo;

    // Leemos para verificar permiso luego
    leer_inodo(ninodo, &inodo) == -1;

    // PERMISO PARA LEER
    if ((inodo.permisos & 4) != 4)
    {
        return -1;
    }

    if (offset >= inodo.tamEnBytesLog)
    {
        // No podemos leer nada
        bytesLeidos = 0;
        return bytesLeidos;
    }

    if ((offset + nbytes) >= inodo.tamEnBytesLog)
    { // Pretende leer más allá de EOF
        nbytes = inodo.tamEnBytesLog - offset;
        // Leemos sólo los bytes que podemos desde el offset hasta EOF
    }

    // DECLARACIONES
    bytesLeidos = 0;
    primerBL = offset / BLOCKSIZE;
    ultimoBL = (offset + nbytes - 1) / BLOCKSIZE;
    desp1 = offset % BLOCKSIZE;
    desp2 = (offset + nbytes - 1) % BLOCKSIZE;

    // CASO A = <Lo que queremos LEER cabe en el bloque físico>
    if (primerBL == ultimoBL)
    {
        nbfisico = traducir_bloque_inodo(ninodo, primerBL, 0);
        if (nbfisico != -1) // si hay bloques físicos para cierto bloque lógico
        {
            bread(nbloque, buf_bloque) == -1);
            memcpy(buf_original, buf_bloque + desp1, nbytes);
        }
        bytesLeidos = nbytes;
    }
    // CASO B = <Lo que queremos LEER ocupa más de un bloque físico>
    else if (primerBL < ultimoBL)
    {
        // 1) Primer bloque lógico
        nbfisico = traducir_bloque_inodo(ninodo, primerBL, 0);
        if (nbfisico != -1)
        {
            if (bread(nbfisico, buf_bloque) == -1)
            {
                return -1
            }
            memcpy(buf_original, buf_bloque + desp1, BLOCKSIZE - desp1);
        }
        bytesLeidos += BLOCKSIZE - desp1;

        // 2) Bloques lógicos intermedios
        for (int i = primerBL + 1; i < ultimoBL; i++)
        {
            nbfisico = traducir_bloque_inodo(ninodo, i, 0); // obtenemos cada bloque intermedio
            if (nbfisico != -1)
            {
                if (bread(nbfisico, buf_bloque) == -1) //****
                {
                    return -1
                }
                memcpy((buf_original + (BLOCKSIZE - desp1) + (i - primerBL - 1) * BLOCKSIZE), buf_bloque, BLOCKSIZE);
            }
            bytesLeidos += BLOCKSIZE;
        }

        // 3) Último bloque lógico
        nbfisico = traducir_bloque_inodo(ninodo, ultimoBL, 0);
        if (nbfisico != -1)
        {
            if (bread(nbfisico, buf_bloque) == -1)
            {
                return -1
            }
            memcpy(buf_original + (nbytes - desp2 - 1), buf_bloque, desp2 + 1);
        }
        bytesLeidos += desp2 + 1;
    }

    // ACTUALIZAMOS METAINFORMACIÓN DEL INODO
    // Leer el inodo actualizado
    leer_inodo(ninodo, &inodo);
    // Actualizar el atime
    inodo.atime = time(NULL);
    // Escribir el inodo
    escribir_inodo(ninodo, inodo);
    // Devolver cantidad de bytes leidos:
    return bytesLeidos;
}

int mi_stat_f(unsigned int ninodo, struct STAT *p_stat) ////////////REVISAR////////////
{
    struct inodo inodo;

    leer_inodo(ninodo, &inodo);
    p_stat->tipo = inodo.tipo;
    p_stat->permisos = inodo.permisos;
    p_stat->nlinks = inodo.nlinks;
    p_stat->tamEnBytesLog = inodo.tamEnBytesLog;
    p_stat->atime = inodo.atime;
    p_stat->ctime = inodo.ctime;
    p_stat->mtime = inodo.mtime;
    p_stat->numBloquesOcupados = inodo.numBloquesOcupados;

    return 1;
}

int mi_chmod_f(unsigned int ninodo, unsigned char permisos)
{
    struct inodo inodo;
    leer_inodo(ninodo, &inodo);
    inodo.permisos = permisos;
    escribir_inodo(ninodo, &inodo);
    inodo.ctime = time(NULL);
    return 1;
}
