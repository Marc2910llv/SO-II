#include "ficheros.h"


int mi_write_f(unsigned int ninodo, const void *buf_original, unsigned int offset, unsigned int nbytes)
{
    //var
    unsigned char *buf_bloque[BLOCKSIZE];
    int primerBL, ultimoBL, desp1, desp2,nbfisico, bytesEscritos;
    
    if ((inodo.permisos & 2) != 2) // necesitamos tener los permisos para escrbibir
    {
        fprintf(stderr," El inodo no tiene permisos necesarios para escribir "\n);
        return EXIT_FAILURE;
    }
        primerBL = offset / BLOCKSIZE; //primer bloque lógico
        ultimoBL = (offset + nbytes - 1) / BLOCKSIZE; //último bloque lógico
        desp1 = offset % BLOCKSIZE; //desplazamiento para el offset
        desp2 = (offset + nbytes - 1) % BLOCKSIZE; // para ver dnd llegan los nbytes
        nbfisico =  traducir_bloque_inodo(ninodo, primerBL, 1);   
        bytesEscritos = 0, aux = 0;

        if (primerBL == ultimoBL) //1) cabe en el bloque físico
        {
            bread(nbfisico, buf_bloque);
            memcpy(buf_bloque + desp1, buf_original, nbytes);
            bwrite(nbfisico, buf_bloque);
            bytesEscritos += nbytes; // si cabe simplemente habremos escrito los nbytes

        }else if(primerBL < ultimoBL) //2) Ocupa más de un bloque físico
        {

         //primer bloque lógico
            bread(nbfisico,buf_bloque);
            memcpy(buf_bloque + desp1, buf_original, BLOCKSIZE-desp1);
            aux = bwrite(nbfisico,buf_bloque);
            bytesEscritos += aux - desp1; //bytes escritos habiendo corrido desp1 y estando situados ya en el primer bloque
        //bloques lógicos intermedios
            for(int i = primerBL + 1; i< ultimoBL;i++)
            {
            nbfisico = traducir_bloque_inodo(ninodo, i, 1); //obtenemos cada bloque intermedio
            aux = bwrite(nbfisico, buf_original + (BLOCKSIZE - desp1) + (i - primerBL - 1) * BLOCKSIZE);  //y lo escribimos
            bytesEscritos += aux;
            }
        //último bloque lógico
            nbfisico =  traducir_bloque_inodo(ninodo, primerBL, 1); 
             bread(nbfisico,buf_bloque);
             memcpy(buf_bloque, buf_original + (nbytes - desp2 - 1), desp2 + 1);
             bwrite(nbfisico, buf_bloque);
            bytesEscritos += desp2 + 1;
        }

        //ACTUALIZAMOS la metainformación del inodo
        leer_inodo(ninodo,&inodo); //Leer el inodo actualizado

        if(inodo.tamEnBytesLog < (nbytes + offset))   //Actualizar el tamaño en bytes lógico (si hemos escrito más allá del final del fichero)
        {
        inodo.ctime = time(NULL); //y por tanto el ctime si modificamos cualquier campo del inodo
        }
        //Actualizar el mtime  (porque hemos escrito en la zona de datos)
        inodo.mtime = time(NULL);
        //Escribir el inodo
        escribir_inodo(ninodo,inodo);
        //Devolver cantidad de bytes escritos:
        return bytesEscritos;
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
