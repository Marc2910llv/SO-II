/*
 *   Marc Llobera Villalonga
 */

#include "ficheros.h"

/// @brief escribir el contenido de buffer de memoria de tamaño nbytes, en un fichero/directorio
/// @param ninodo inodo correspondiente al fichero/directorio
/// @param buf_original buffer de memoria
/// @param offset posición de escritura inicial
/// @param nbytes bytes a escribir
/// @return
int mi_write_f(unsigned int ninodo, const void *buf_original, unsigned int offset, unsigned int nbytes)
{
    if ((inodo.permisos & 2) != 2)
    {
        perror("No se tienen los permisos necesarios");
        return -2;
    }

    int primerBL = offset / BLOCKSIZE;
    int ultimoBL = (offset + nbytes - 1) / BLOCKSIZE;
    int desp1 = offset % BLOCKSIZE;
    int desp2 = (offset + nbytes - 1) % BLOCKSIZE;

    if (primerBL == ultimoBL) // cabe solo un bloque
    {
    }
    else // cabe más de un bloque
    {
    }
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