/*
 *   Marc Llobera Villalonga
 */

#include "ficheros.h"

/// @brief escribir el contenido de buffer de memoria de tamaño nbytes, en un fichero/directorio
/// @param ninodo inodo correspondiente al fichero/directorio
/// @param buf_original buffer de memoria
/// @param offset posición de escritura inicial
/// @param nbytes bytes a escribir
/// @return bytes escritos
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
    size_t bytesEscritosAux;

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

        bytesEscritosAux = bwrite(nbfisico, buf_bloque);
        if (bytesEscritosAux == FALLO)
        {
            perror("Error mi_write_f bwrite (primerBL == ultimoBL)");
            return FALLO;
        }

        bytesEscritos = bytesEscritos + bytesEscritosAux;
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

        bytesEscritosAux = bwrite(nbfisico, buf_bloque);
        if (bytesEscritosAux == FALLO)
        {
            perror("Error mi_write_f bwrite (PRIMERA FASE)");
            return FALLO;
        }

        bytesEscritos = bytesEscritos + bytesEscritosAux;

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

            bytesEscritosAux = bwrite(nbfisico, buf_original + (BLOCKSIZE - desp1) + (i - primerBL - 1) * BLOCKSIZE);
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

        bytesEscritosAux = bwrite(nbfisico, buf_bloque);
        if (bytesEscritosAux == FALLO)
        {
            perror("Error mi_write_f bwrite (TERCERA FASE)");
            return FALLO;
        }

        bytesEscritos = bytesEscritos + bytesEscritosAux;
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

/// @brief
/// @param ninodo
/// @param buf_original
/// @param offset
/// @param nbytes
/// @return
int mi_read_f(unsigned int ninodo, void *buf_original, unsigned int offset,
              unsigned int nbytes)
{
}

/// @brief
/// @param ninodo
/// @param p_stat
/// @return
int mi_stat_f(unsigned int ninodo, struct STAT *p_stat)
{
}

/// @brief
/// @param ninodo
/// @param permisos
/// @return
int mi_chmod_f(unsigned int ninodo, unsigned char permisos)
{
}