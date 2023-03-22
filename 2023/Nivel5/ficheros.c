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
    union _indodo inodo;
    if (leer_inodo(ninodo, &inodo) == FALLO)
    {
        perror("Error mi_write_f leer_inodo (inicio)");
        return FALLO;
    }

    if ((inodo.permisos & 2) != 2)
    {
        perror("No se tienen los permisos necesarios");
        return -2;
    }

    int primerBL = offset / BLOCKSIZE;
    int ultimoBL = (offset + nbytes - 1) / BLOCKSIZE;
    int desp1 = offset % BLOCKSIZE;
    int desp2 = (offset + nbytes - 1) % BLOCKSIZE;

    int nbfisico = FALLO;
    unsigned char buf_bloque[BLOCKSIZE];

    size_t bytesEscritos = 0;

    if (primerBL == ultimoBL) // cabe solo un bloque
    {
        nbfisico = traducir_bloque_inodo(ninodo, primerBL, 1); // Obtenemos el nº de bloque físico
        if (nbfisico == FALLO)
        {
            perror("Error mi_write_f traducir_bloque_inodo (primerBL == ultimoBL)");
            return FALLO;
        }

        if (bread(nbfisico, buf_bloque) == FALLO)
        {
            perror("Error mi_write_f bread (primerBL == ultimoBL)");
            return FALLO;
        }

        memcpy(buf_bloque + desp1, buf_original, nbytes); // Utilizamos memcpy() para escribir los nbytes

        if (bwrite(nbfisico, buf_bloque) == FALLO)
        {
            perror("Error mi_write_f bwrite (primerBL == ultimoBL)");
            return FALLO;
        }

        bytesEscritos = bytesEscritos + nbytes;
    }
    else if (primerBL < ultimoBL) // cabe más de un bloque
    {
        // 1º PRIMERA FASE
        // Primer bloque lógico
        nbfisico = traducir_bloque_inodo(ninodo, primerBL, 1); // Obtenemos el nº de bloque físico
        if (nbfisico == FALLO)
        {
            perror("Error mi_write_f traducir_bloque_inodo (PRIMERA FASE)");
            return FALLO;
        }

        if (bread(nbfisico, buf_bloque) == FALLO)
        {
            perror("Error mi_write_f bread (PRIMERA FASE)");
            return FALLO;
        }

        memcpy(buf_bloque + desp1, buf_original, BLOCKSIZE - desp1);

        if (bwrite(nbfisico, buf_bloque) == FALLO)
        {
            perror("Error mi_write_f bwrite (PRIMERA FASE)");
            return FALLO;
        }

        bytesEscritos = bytesEscritos + (BLOCKSIZE - desp1);

        // 2º SEGUNDA FASE
        // Bloques lógicos intermedios
        for (int i = primerBL + 1; i < ultimoBL; i++)
        {
            nbfisico = traducir_bloque_inodo(ninodo, i, 1); // Obtenemos el nº de bloque físico
            if (nbfisico == FALLO)
            {
                perror("Error mi_write_f traducir_bloque_inodo (SEGUNDA FASE)");
                return FALLO;
            }

            size_t bytesEscritosAux = bwrite(nbfisico, buf_original + (BLOCKSIZE - desp1) + (i - primerBL - 1) * BLOCKSIZE);
            if (bytesEscritosAux == FALLO)
            {
                perror("Error mi_write_f bwrite (SEGUNDA FASE)");
                return FALLO;
            }

            bytesEscritos = bytesEscritos + bytesEscritosAux;
        }

        // 3º TERCERA FASE
        // Último bloque lógico
        nbfisico = traducir_bloque_inodo(ninodo, primerBL, 1); // Obtenemos el nº de bloque físico
        if (nbfisico == FALLO)
        {
            perror("Error mi_write_f traducir_bloque_inodo (TERCERA FASE)");
            return FALLO;
        }

        if (bread(nbfisico, buf_bloque) == FALLO)
        {
            perror("Error mi_write_f bread (TERCERA FASE)");
            return FALLO;
        }

        memcpy(buf_bloque, buf_original + (nbytes - (desp2 + 1)), desp2 + 1);

        if (bwrite(nbfisico, buf_bloque) == FALLO)
        {
            perror("Error mi_write_f bwrite (TERCERA FASE)");
            return FALLO;
        }

        bytesEscritos = bytesEscritos + (desp2 + 1);
    }
    else
    {
        perror("Error la entrada de parámetros de la función és incorrecta");
        return FALLO;
    }

    // Actualizar Metainformación
    if (leer_inodo(ninodo, &inodo) == FALLO)
    {
        perror("Error mi_write_f leer_inodo (final)");
        return FALLO;
    }

    if (inodo.tamEnBytesLog < (nbytes + offset))
    {
        inodo.tamEnBytesLog = nbytes + offset;
        inodo.ctime = time(NULL);
    }

    inodo.mtime = time(NULL);

    if (escribir_inodo(ninodo, inodo) == FALLO)
    {
        perror("Error mi_write_f escribir_inodo");
        return FALLO;
    }

    return bytesEscritos;
}

/// @brief leer información de un fichero/directorio y almacenarla en un buffer de memoria
/// @param ninodo inodo correspondiente al fichero/directorio
/// @param buf_original buffer de memoria
/// @param offset posición de lectura inicial
/// @param nbytes número de bytes a leer
/// @return bytes leídos o -2 (no se tienen permisos) o FALLO
int mi_read_f(unsigned int ninodo, void *buf_original, unsigned int offset,
              unsigned int nbytes)
{
    union _indodo inodo;
    if (leer_inodo(ninodo, &inodo) == FALLO)
    {
        perror("Error mi_read_f leer_inodo (inicio)");
        return FALLO;
    }

    if ((inodo.permisos & 4) != 4)
    {
        perror("No se tienen los permisos necesarios");
        return -2;
    }

    size_t bytesLeidos;

    if (offset >= inodo.tamEnBytesLog)
    {
        bytesLeidos = 0; // No podemos leer nada
        return bytesLeidos;
    }
    if ((offset + nbytes) >= inodo.tamEnBytesLog)
    { // pretende leer más allá de EOF
        nbytes = inodo.tamEnBytesLog - offset;
        // leemos sólo los bytes que podemos desde el offset hasta EOF
    }

    int primerBL = offset / BLOCKSIZE;
    int ultimoBL = (offset + nbytes - 1) / BLOCKSIZE;
    int desp1 = offset % BLOCKSIZE;
    int desp2 = (offset + nbytes - 1) % BLOCKSIZE;

    int nbfisico = FALLO;
    unsigned char buf_bloque[BLOCKSIZE];

    if (primerBL == ultimoBL) // cabe solo un bloque
    {
        nbfisico = traducir_bloque_inodo(ninodo, primerBL, 0); // Obtenemos el nº de bloque físico
        if (nbfisico == FALLO)
        {
            if (bread(nbfisico, buf_bloque) == FALLO)
            {
                perror("Error mi_read_f bread (primerBL == ultimoBL)");
                return FALLO;
            }

            memcpy(buf_bloque + desp1, buf_original, nbytes); // Utilizamos memcpy() para escribir los nbytes
        }

        bytesLeidos = bytesLeidos + nbytes;
    }
    else if (primerBL < ultimoBL) // cabe más de un bloque
    {
        // 1º PRIMERA FASE
        // Primer bloque lógico
        nbfisico = traducir_bloque_inodo(ninodo, primerBL, 0); // Obtenemos el nº de bloque físico
        if (nbfisico == FALLO)
        {
            if (bread(nbfisico, buf_bloque) == FALLO)
            {
                perror("Error mi_read_f bread (PRIMERA FASE)");
                return FALLO;
            }

            memcpy(buf_bloque + desp1, buf_original, BLOCKSIZE - desp1);
        }

        bytesLeidos = bytesLeidos + (BLOCKSIZE - desp1);

        // 2º SEGUNDA FASE
        // Bloques lógicos intermedios
        for (int i = primerBL + 1; i < ultimoBL; i++)
        {
            nbfisico = traducir_bloque_inodo(ninodo, i, 0); // Obtenemos el nº de bloque físico
            if (nbfisico == FALLO)
            {
                if (bread(nbfisico, buf_bloque) == FALLO)
                {
                    perror("Error EL mi_read_f bread (SEGUNDA FASE)");
                    return FALLO;
                }

                memcpy((buf_original + (BLOCKSIZE - desp1) + (i - primerBL - 1) * BLOCKSIZE), buf_bloque, BLOCKSIZE);
            }

            bytesLeidos = bytesLeidos + BLOCKSIZE;
        }

        // 3º TERCERA FASE
        // Último bloque lógico
        nbfisico = traducir_bloque_inodo(ninodo, primerBL, 0); // Obtenemos el nº de bloque físico
        if (nbfisico == FALLO)
        {
            if (bread(nbfisico, buf_bloque) == FALLO)
            {
                perror("Error mi_read_f bread (TERCERA FASE)");
                return FALLO;
            }

            memcpy(buf_bloque, buf_original + (nbytes - (desp2 + 1)), desp2 + 1);
        }

        bytesLeidos = bytesLeidos + (desp2 + 1);
    }
    else
    {
        perror("Error la entrada de parámetros de la función és incorrecta");
        return FALLO;
    }

    // Actualizar Metainformación
    if (leer_inodo(ninodo, &inodo) == FALLO)
    {
        perror("Error mi_read_f leer_inodo (final)");
        return FALLO;
    }

    inodo.atime = time(NULL);

    if (escribir_inodo(ninodo, inodo) == FALLO)
    {
        perror("Error mi_read_f escribir_inodo");
        return FALLO;
    }

    return bytesLeidos;
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