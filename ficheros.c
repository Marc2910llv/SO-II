#include "ficheros.h"

int mi_write_f(unsigned int ninodo, const void *buf_original, unsigned int offset, unsigned int nbytes)
{
    if ((inodo.permisos & 2) != 2)
    {
        int primerBL = offset / BLOCKSIZE;
        int ultimoBL = (offset + nbytes - 1) / BLOCKSIZE;
        int desp1 = offset % BLOCKSIZE;
        int desp2 = (offset + nbytes - 1) % BLOCKSIZE;

        if (primerBL == ultimoBL)
        {
            int nbfisico = traducir_bloque_inodo(ninodo, primerBL, 1);
            unsigned char *buf_bloque[BLOCKSIZE];
            bread(nbfisico, buf_bloque);
            memcpy(buf_bloque + desp1, buf_original, nbytes);
            bwrite(nbfisico, buf_bloque);
        }
    }
}

int mi_read_f(unsigned int ninodo, void *buf_original, unsigned int offset, unsigned int nbytes)
{
    if ((inodo.permisos & 4) != 4)
    {
        if (offset >= inodo.tamEnBytesLog)
        {
            int leidos = 0; // No podemos leer nada
            return leidos;
        }
        if ((offset + nbytes) >= inodo.tamEnBytesLog)
        { // pretende leer más allá de EOF
            nbytes = inodo.tamEnBytesLog - offset;
            // leemos sólo los bytes que podemos desde el offset hasta EOF
        }
    }
}

int mi_stat_f(unsigned int ninodo, struct STAT *p_stat)
{
    struct inodo inodo;
    if (leer_inodo(ninodo, &inodo))
    {
        perror("Error");
        return -1;
    }
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

    return 1;
}
