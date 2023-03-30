/*
 *   Marc Llobera Villalonga
 */

#include "ficheros.h"

/// @brief escribir el contenido de buffer de memoria de tamaño nbytes, en un fichero/directorio
/// @param ninodo inodo correspondiente al fichero/directorio
/// @param buf_original buffer de memoria
/// @param offset posición de escritura inicial
/// @param nbytes bytes a escribir
/// @return bytes escritos o -2 (no se tienen permisos) o FALLO
int mi_write_f(unsigned int ninodo, const void *buf_original, unsigned int offset, unsigned int nbytes)
{
    char unsigned buf_bloque[BLOCKSIZE];
    union _inodo inodo;

    if (leer_inodo(ninodo, &inodo) == FALLO)
    {
        fprintf(stderr, "Error mi_write_f leer_inodo\n");
        return FALLO;
    }

    if ((inodo.permisos & 2) != 2)
    {
        fprintf(stderr, "El inodo no tiene permisos para escribir\n");
        return FALLO;
    }

    unsigned int primerBL = offset / BLOCKSIZE;
    unsigned int ultimoBL = (offset + nbytes - 1) / BLOCKSIZE;

    int desp1 = offset % BLOCKSIZE;
    int desp2 = (offset + nbytes - 1) % BLOCKSIZE;

    int nbfisico = traducir_bloque_inodo(ninodo, primerBL, 1);
    if (nbfisico == FALLO)
    {
        fprintf(stderr, "Error mi_write_f traducir_bloque_inodo\n");
        return FALLO;
    }

    if (bread(nbfisico, buf_bloque) == FALLO)
    {
        fprintf(stderr, "Error mi_write_f bread\n");
        return FALLO;
    }

    int bytesescritos = 0;
    int auxByteEscritos = 0;

    if (primerBL == ultimoBL) // cabe un solo bloque
    {
        memcpy(buf_bloque + desp1, buf_original, nbytes);

        auxByteEscritos = bwrite(nbfisico, buf_bloque);
        if (auxByteEscritos == FALLO)
        {
            fprintf(stderr, "Error mi_write_f bwrite\n");
            return FALLO;
        }
        bytesescritos += nbytes;
    }
    else if (primerBL < ultimoBL) // cabe más de un bloque
    {
        // 1º PRIMERA FASE
        // Primer bloque lógico

        memcpy(buf_bloque + desp1, buf_original, BLOCKSIZE - desp1);

        auxByteEscritos = bwrite(nbfisico, buf_bloque);
        if (auxByteEscritos == FALLO)
        {
            fprintf(stderr, "Error mi_write_f bwrite\n");
            return FALLO;
        }
        bytesescritos = bytesescritos + (auxByteEscritos - desp1);

        // 2º SEGUNDA FASE
        // Bloques lógicos intermedios

        for (int i = primerBL + 1; i < ultimoBL; i++)
        {
            nbfisico = traducir_bloque_inodo(ninodo, i, 1);
            if (nbfisico == FALLO)
            {
                fprintf(stderr, "Error mi_write_f traducir_bloque_inodo\n");
                return FALLO;
            }

            auxByteEscritos = bwrite(nbfisico, buf_original + (BLOCKSIZE - desp1) + (i - primerBL - 1) * BLOCKSIZE);
            if (auxByteEscritos == FALLO)
            {
                fprintf(stderr, "Error mi_write_f bwrite\n");
                return FALLO;
            }
            bytesescritos = bytesescritos + auxByteEscritos;
        }

        // 3º TERCERA FASE
        // Último bloque lógico

        nbfisico = traducir_bloque_inodo(ninodo, ultimoBL, 1);
        if (nbfisico == FALLO)
        {
            fprintf(stderr, "Error mi_write_f traducir_bloque_inodo\n");
            return FALLO;
        }

        if (bread(nbfisico, buf_bloque) == FALLO)
        {
            fprintf(stderr, "Error mi_write_f bread\n");
            return FALLO;
        }

        memcpy(buf_bloque, buf_original + (nbytes - desp2 - 1), desp2 + 1);

        auxByteEscritos = bwrite(nbfisico, buf_bloque);
        if (auxByteEscritos == FALLO)
        {
            fprintf(stderr, "Error mi_write_f bwrite\n");
            return FALLO;
        }

        bytesescritos = bytesescritos + (desp2 + 1);
    }

    if (leer_inodo(ninodo, &inodo) == FALLO)
    {
        fprintf(stderr, "Error mi_write_f leer_inodo\n");
        return FALLO;
    }

    // Actualizar Metainformación

    if (inodo.tamEnBytesLog < (nbytes + offset))
    {
        inodo.tamEnBytesLog = nbytes + offset;
        inodo.ctime = time(NULL);
    }

    inodo.mtime = time(NULL);

    if (escribir_inodo(ninodo, inodo) == FALLO)
    {
        fprintf(stderr, "Error mi_write_f escribir_inodo\n");
        return FALLO;
    }

    if (nbytes == bytesescritos)
    {
        return bytesescritos;
    }
    else
    {
        return FALLO;
    }
}

/// @brief leer información de un fichero/directorio y almacenarla en un buffer de memoria
/// @param ninodo inodo correspondiente al fichero/directorio
/// @param buf_original buffer de memoria
/// @param offset posición de lectura inicial
/// @param nbytes número de bytes a leer
/// @return bytes leídos o -2 (no se tienen permisos) o FALLO
int mi_read_f(unsigned int ninodo, void *buf_original, unsigned int offset, unsigned int nbytes)
{
    char unsigned buf_bloque[BLOCKSIZE];
    union _inodo inodo;

    if (leer_inodo(ninodo, &inodo) == FALLO)
    {
        fprintf(stderr, "Error mi_read_f leer_inodo\n");
        return FALLO;
    }

    if ((inodo.permisos & 4) != 4)
    {
        fprintf(stderr, "El inodo no tiene permisos para escribir\n");
        return FALLO;
    }

    int bytesleidos = 0;
    int auxLeidos = 0;

    if (offset >= inodo.tamEnBytesLog)
    {
        return bytesleidos;
    }

    if ((offset + nbytes) >= inodo.tamEnBytesLog)
    {
        nbytes = inodo.tamEnBytesLog - offset;
    }

    unsigned int primerBL = offset / BLOCKSIZE;
    unsigned int ultimoBL = (offset + nbytes - 1) / BLOCKSIZE;

    int desp1 = offset % BLOCKSIZE;
    int desp2 = (offset + nbytes - 1) % BLOCKSIZE;

    int nbfisico = traducir_bloque_inodo(ninodo, primerBL, 0);
    if (primerBL == ultimoBL)
    {
        if (nbfisico != FALLO)
        {
            auxLeidos = bread(nbfisico, buf_bloque);
            if (auxLeidos == FALLO)
            {
                fprintf(stderr, "Error mi_read_f bread\n");
                return FALLO;
            }
            memcpy(buf_original, buf_bloque + desp1, nbytes);
        }

        bytesleidos = nbytes;
    }
    else if (primerBL < ultimoBL)
    {
        // Parte 1: Primero bloque leido parcialmente
        if (nbfisico != FALLO)
        {
            auxLeidos = bread(nbfisico, buf_bloque);
            if (auxLeidos == FALLO)
            {
                fprintf(stderr, "Error mi_read_f bread\n");
                return FALLO;
            }
            memcpy(buf_original, buf_bloque + desp1, BLOCKSIZE - desp1);
        }

        bytesleidos = BLOCKSIZE - desp1;

        // Parte 2: Bloques intermedios
        for (int i = primerBL + 1; i < ultimoBL; i++)
        {
            nbfisico = traducir_bloque_inodo(ninodo, i, 0);
            if (nbfisico != FALLO)
            {
                auxLeidos = bread(nbfisico, buf_bloque);
                if (auxLeidos == FALLO)
                {
                    fprintf(stderr, "Error mi_read_f bread\n");
                    return FALLO;
                }
                memcpy(buf_original + (BLOCKSIZE - desp1) + (i - primerBL - 1) * BLOCKSIZE, buf_bloque, BLOCKSIZE);
            }

            bytesleidos = bytesleidos + BLOCKSIZE;
        }

        // Parte 3: Último bloque leido parcialmente
        // Obtenemos el bloque fisico
        nbfisico = traducir_bloque_inodo(ninodo, ultimoBL, 0);
        if (nbfisico != FALLO)
        {
            auxLeidos = bread(nbfisico, buf_bloque);
            if (auxLeidos == FALLO)
            {
                fprintf(stderr, "Error mi_read_f bread\n");
                return FALLO;
            }
            memcpy(buf_original + (nbytes - desp2 - 1), buf_bloque, desp2 + 1);
        }

        bytesleidos = bytesleidos + (desp2 + 1);
    }

    if (leer_inodo(ninodo, &inodo) == FALLO)
    {
        fprintf(stderr, "Error mi_read_f leer_inodo\n");
        return FALLO;
    }

    inodo.atime = time(NULL);

    if (escribir_inodo(ninodo, inodo) == FALLO)
    {
        fprintf(stderr, "Error mi_read_f escribir_inodo\n");
        return FALLO;
    }

    if (nbytes == bytesleidos)
    {
        return bytesleidos;
    }
    else
    {
        return FALLO;
    }
}

/// @brief devolver la metainformación de un fichero/directorio
/// @param ninodo inodo correspondiente al fichero/directorio
/// @param p_stat
/// @return
int mi_stat_f(unsigned int ninodo, struct STAT *p_stat)
{
    union _inodo inodo;
    if (leer_inodo(ninodo, &inodo) == FALLO)
    {
        perror("Error mi_stat_f leer_inodo");
        return FALLO;
    }

    p_stat->tipo = inodo.tipo;
    p_stat->permisos = inodo.permisos;
    p_stat->nlinks = inodo.nlinks;
    p_stat->tamEnBytesLog = inodo.tamEnBytesLog;
    p_stat->atime = inodo.atime;
    p_stat->ctime = inodo.ctime;
    p_stat->mtime = inodo.mtime;
    p_stat->numBloquesOcupados = inodo.numBloquesOcupados;

    return EXITO;
}

/// @brief cambiar los permisos de un fichero/directorio
/// @param ninodo inodo correspondiente al fichero/directorio
/// @param permisos nuevos permisos a asignar
/// @return
int mi_chmod_f(unsigned int ninodo, unsigned char permisos)
{
    union _inodo inodo;
    if (leer_inodo(ninodo, &inodo) == FALLO)
    {
        perror("Error mi_chmod_f leer_inodo");
        return FALLO;
    }

    inodo.permisos = permisos;
    inodo.ctime = time(NULL);

    if (escribir_inodo(ninodo, inodo) == FALLO)
    {
        perror("Error mi_chmod_f escribir_inodo");
        return FALLO;
    }

    return EXITO;
}

/// @brief
/// @param ninodo
/// @param nbytes
/// @return
int mi_truncar_f(unsigned int ninodo, unsigned int nbytes)
{
    union _inodo inodo;
    if (leer_inodo(ninodo, &inodo) == FALLO)
    {
        perror("Error mi_truncar_f leer_inodo");
        return FALLO;
    }

    if ((inodo.permisos & 2) != 2)
    {
        perror("Error mi_truncar_f, no se tienen los permisos necesarios");
        return FALLO;
    }

    if (nbytes > inodo.tamEnBytesLog)
    {
        perror("Error mi_truncar_f, hay más nbytes que el tamaño en bytes lógicos");
        return FALLO;
    }

    int primerBL;
    if (nbytes % BLOCKSIZE == 0)
    {
        primerBL = nbytes / BLOCKSIZE;
    }
    else
    {
        primerBL = nbytes / BLOCKSIZE + 1;
    }

    int bloquesLiberados = liberar_bloques_inodo(primerBL, &inodo);
    if (bloquesLiberados < 1)
    {
        perror("Error mi_truncar_f liberar_bloques_inodo");
        return FALLO;
    }

    inodo.mtime = time(NULL);
    inodo.ctime = time(NULL);
    inodo.tamEnBytesLog = nbytes;
    inodo.numBloquesOcupados = inodo.numBloquesOcupados - bloquesLiberados;

    if (escribir_inodo(ninodo, inodo) == FALLO)
    {
        perror("Error mi_truncar_f escribir_inodo");
        return FALLO;
    }

    return bloquesLiberados;
}