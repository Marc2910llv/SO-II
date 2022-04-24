#include "ficheros.h"
#include <time.h>

// Escribe el contenido procedente de un buffer de memoria, buf_original, de tamaño nbytes, en un fichero/directorio.
int mi_write_f(unsigned int ninodo, const void *buf_original, unsigned int offset, unsigned int nbytes)
{
    unsigned char buf_bloque[BLOCKSIZE];
    int primerBL, ultimoBL, desp1, desp2, nbfisico, intermitjos;
    size_t bytesEscritos = 0;

    struct inodo inodo;
    if (leer_inodo(ninodo, &inodo) != 0)
    {
        perror("ERROR EN mi_write_f AL LEER EL INODO");
        return -1;
    }

    if ((inodo.permisos & 2) != 2) // Comprobamos si tenemos permisos de escritura
    {
        perror("NO HAY PERMISO DE ESCRITURA");
        return -1;
    }

    primerBL = offset / BLOCKSIZE;
    ultimoBL = (offset + nbytes - 1) / BLOCKSIZE;
    desp1 = offset % BLOCKSIZE;
    desp2 = (offset + nbytes - 1) % BLOCKSIZE;

    // CASO 1º (el buffer cabe en un solo bloque)
    if (primerBL == ultimoBL)
    {
        nbfisico = traducir_bloque_inodo(ninodo, primerBL, 1); // Obtenemos el nº de bloque físico
        if (bread(nbfisico, buf_bloque) == -1)
        {
            perror("ERROR EN mi_write_f AL LEER EL BLOQUE FÍSICO EN EL CASO 1");
            return -1;
        }
        memcpy(buf_bloque + desp1, buf_original, nbytes); // Utilizamos memcpy() para escribir los nbytes
        if (bwrite(nbfisico, buf_bloque) == -1)
        {
            perror("ERROR EN mi_write_f AL ESCRIBIR EL BLOQUE FÍSICO EN EL CASO 1");
            return -1;
        }

        bytesEscritos = bytesEscritos + nbytes;
    }
    // CASO 2º (la operación de escritura ocupa más de un bloque)
    else if (primerBL < ultimoBL)
    {
        // 1) Primer bloque lógico (BL8)
        nbfisico = traducir_bloque_inodo(ninodo, primerBL, 1); // Obtenemos el nº de bloque físico
        if (bread(nbfisico, buf_bloque) == -1)
        {
            perror("ERROR EN mi_write_f AL LEER EL BLOQUE FÍSICO EN EL CASO 2 PRIMER BLOQUE LÓGICO");
            return -1;
        }
        memcpy(buf_bloque + desp1, buf_original, BLOCKSIZE - desp1);
        if (bwrite(nbfisico, buf_bloque) == -1)
        {
            perror("ERROR EN mi_write_f AL ESCRIBIR EL BLOQUE FÍSICO EN EL CASO 2 PRIMER BLOQUE LÓGICO");
            return -1;
        }

        // Bytes escritos habiendo corrido desp1 y estando situados ya en el primer bloque
        bytesEscritos = bytesEscritos + BLOCKSIZE - desp1;

        // 2) Bloques lógicos intermedios (bloques 9, 10, 11)
        for (int i = primerBL + 1; i < ultimoBL; i++)
        {
            // Obtenemos cada bloque intermedio
            nbfisico = traducir_bloque_inodo(ninodo, i, 1);
            intermitjos = bwrite(nbfisico, buf_original + (BLOCKSIZE - desp1) + (i - primerBL - 1) * BLOCKSIZE);
            if (intermitjos == -1)
            {
                perror("ERROR EN mi_write_f AL ESCRIBIR EN LOS BLOQUES LÓGICOS INTERMEDIOS, CASO 2");
                return -1;
            }

            bytesEscritos = bytesEscritos + intermitjos;
        }

        // 3) Último bloque lógico (bloque 12)
        nbfisico = traducir_bloque_inodo(ninodo, ultimoBL, 1);
        if (bread(nbfisico, buf_bloque) == -1)
        {
            perror("ERROR EN mi_write_f AL LEER EL BLOQUE FÍSICO EN EL CASO 2 ÚLTIMO BLOQUE LÓGICO");
            return -1;
        }
        memcpy(buf_bloque, buf_original + (nbytes - desp2 - 1), desp2 + 1);
        if (bwrite(nbfisico, buf_bloque) == -1)
        {
            perror("ERROR EN mi_write_f AL ESCRIBIR EL BLOQUE FÍSICO EN EL CASO 2 ÚLTIMO BLOQUE LÓGICO");
            return -1;
        }

        bytesEscritos = bytesEscritos + desp2 + 1;
    }

    // ACTUALIZAMOS METAINFORMACIÓN DEL INODO
    if (leer_inodo(ninodo, &inodo) == -1)
    {
        perror("ERROR EN mi_write_f AL LEER EL INODO CUANDO QUEREMOS ACTUALIZAR LA METAINFORMACIÓN");
        return -1;
    }
    // Actualizar el tamaño en bytes lógico (si hemos escrito más allá del final del fichero)
    if (inodo.tamEnBytesLog < (nbytes + offset))
    {
        inodo.tamEnBytesLog = nbytes + offset;
        inodo.ctime = time(NULL);
    }

    inodo.mtime = time(NULL);

    if (escribir_inodo(ninodo, inodo) == -1)
    {
        perror("ERROR EN mi_write_f AL ESCRIBIR EL INODO CUANDO QUEREMOS ACTUALIZAR LA METAINFORMACIÓN");
        return -1;
    }

    return bytesEscritos;
}

// Lee información de un fichero/directorio y la almacena en un buffer de memoria, buf_original
int mi_read_f(unsigned int ninodo, void *buf_original, unsigned int offset, unsigned int nbytes) //////////////REVISAR//////////////////
{
    // VARIABLES
    unsigned char buf_bloque[BLOCKSIZE];
    int primerBL, ultimoBL, desp1, desp2, nbfisico, bytesLeidos;
    struct inodo inodo;
    bytesLeidos = 0;

    // Leemos para verificar permiso luego
    if (leer_inodo(ninodo, &inodo) == -1)
    {
        perror("ERROR EN mi_read_f AL LEER EL INODO PARA VERIFICAR EL PERMISO");
        return -1;
    }

    // PERMISO PARA LEER
    if ((inodo.permisos & 4) != 4)
    {
        return bytesLeidos;
    }

    if (offset >= inodo.tamEnBytesLog)
    {
        // No podemos leer nada
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
            if (bread(nbfisico, buf_bloque) == -1)
            {
                perror("ERROR EL mi_read_f AL LEER EN EL CASO DE QUE CABE EN EL BLOQUE FÍSICO");
                return -1;
            }
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
                perror("ERROR EL mi_read_f AL LEER EN EL PRIMER BLOQUE LÓGICO");
                return -1;
            }
            memcpy(buf_original, buf_bloque + desp1, BLOCKSIZE - desp1);
        }
        bytesLeidos = BLOCKSIZE - desp1;

        // 2) Bloques lógicos intermedios
        for (int i = primerBL + 1; i < ultimoBL; i++)
        {
            nbfisico = traducir_bloque_inodo(ninodo, i, 0); // obtenemos cada bloque intermedio
            if (nbfisico != -1)
            {
                if (bread(nbfisico, buf_bloque) == -1) //****
                {
                    perror("ERROR EL mi_read_f AL LEER EN LOS BLOQUES LÓGICOS INTERMEDIOS");
                    return -1;
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
                perror("ERROR EL mi_read_f AL LEER EN EL ÚLTIMO BLOQUE LÓGICO");
                return -1;
            }
            memcpy(buf_original + (nbytes - desp2 - 1), buf_bloque, desp2 + 1);
        }
        bytesLeidos += desp2 + 1;
    }

    // ACTUALIZAMOS METAINFORMACIÓN DEL INODO
    // Leer el inodo actualizado
    if (leer_inodo(ninodo, &inodo) == -1)
    {
        perror("ERROR EN mi_read_f AL LEER EL INODO CUANDO QUEREMOS ACTUALIZAR LA METAINFORMACIÓN");
        return -1;
    }
    // Actualizar el atime
    inodo.atime = time(NULL);
    // Escribir el inodo
    if (escribir_inodo(ninodo, inodo) == -1)
    {
        perror("ERROR EN mi_read_f AL ESCRIBIR EL INODO CUANDO QUEREMOS ACTUALIZAR LA METAINFORMACIÓN");
        return -1;
    }
    // Devolver cantidad de bytes leidos:
    return bytesLeidos;
}

int mi_stat_f(unsigned int ninodo, struct STAT *p_stat)
{
    struct inodo inodo;
    if (leer_inodo(ninodo, &inodo) == -1)
    {
        perror("ERROR EN mi_stat_f AL LEER EL INODO");
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

int mi_chmod_f(unsigned int ninodo, unsigned char permisos) ////////////REVISAR////////////
{
    struct inodo inodo;
    if (leer_inodo(ninodo, &inodo) == -1)
    {
        perror("ERROR EN mi_chmod_f AL LEER EL INODO");
        return -1;
    }
    inodo.permisos = permisos;
    if (escribir_inodo(ninodo, inodo) == -1)
    {
        perror("ERROR EN mi_chmod_f AL ESCRIBIR EL INODO");
        return -1;
    }
    inodo.ctime = time(NULL);
    return 1;
}

// Trunca un fichero/directorio a los bytes indicados como nbytes, liberando los bloques necesarios
int mi_truncar_f(unsigned int ninodo, unsigned int nbytes)
{
    int i, liberar;
    struct inodo inodo;
    if (leer_inodo(ninodo, &inodo) == -1)
    {
        perror("ERROR EN mi_truncar_f AL LEER EL INODO");
    }

    if ((inodo.permisos & 2) != 2)
    {
        perror("ERROR EN mi_truncar_f, FALTAN PERMISOS");
        return -1
    }

    if (nbytes > inodo.tamEnBytesLog)
    {
        perror("ERROR EN mi_truncar_f, NBYTES MAYOR QUE EL TAMAÑO EN BYTES LÓGICOS");
        return -1;
    }

    if (nbytes % BLOCKSIZE == 0)
    {
        i = nbytes / BLOCKSIZE;
    }
    else
    {
        i = (nbytes / BLOCKSIZE) + 1;
    }

    liberar = liberar_bloques_inodo(i, &inodo);
    if (liberar == -1)
    {
        perror("ERROR EN mi_truncar_f AL LIBERAR LOS BLOQUES OCUPADOS");
        return -1;
    }
    inodo.mtime = time(NULL);
    inodo.ctime = time(NULL);
    inodo.tamEnBytesLog = nbytes;
    inodo.numBloquesOcupados = inodo.numBloquesOcupados - liberar;

    if (escribir_inodo(ninodo, inodo) == -1)
    {
        perror("ERROR EN mi_truncar_f AL ESCRIBIR EL INODO");
        return -1;
    }

    return liberar;
}