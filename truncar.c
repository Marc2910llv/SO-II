#include "ficheros.h"
#include<stdio.h>

int main(int argc, char *argv[])
{
    int nbytes, ninodo;
    //struct inodo inodo;
    struct STAT stat;

    if (argc != 4)
    {
        printf("Sintaxis correcta: truncar <nombre_dispositivo> <ninodo> <nbytes>\n");
    }
    else
    {
        if (bmount(argv[1]) == -1)
        {
            perror("Error a l'hora de montar el sistema\n");
            return -1;
        }

        nbytes = atoi(argv[3]);
        ninodo = atoi(argv[2]);
        if (nbytes == 0)
        {
            liberar_inodo(ninodo);
        }
        else
        {
            mi_truncar_f(ninodo, nbytes);
        }
        //leer_inodo(ninodo, &inodo);
        mi_stat_f(ninodo,&stat);
        //char *t1,*t2,*t3;
        char atime [100];
        char ctime[100];
        char mtime[100];

        time_t a,b,c;
        struct tm *t1,*t2,*t3;
        a = stat.atime;
        b= stat.ctime;
        c= stat.mtime;

        t1=localtime(&a);
        t2=localtime(&b);
        t3=localtime(&c);

        strftime(atime, 100, "%a %Y-%m-%d %H:%M:%S",t1);
        strftime(ctime, 100, "%a %Y-%m-%d %H:%M:%S",t2);
        strftime(mtime, 100, "%a %Y-%m-%d %H:%M:%S",t3);

        printf("DATOS INODO %d\n", ninodo);
        printf("tipo= %c\n", stat.tipo);
        printf("permisos= %d\n", stat.permisos);
        printf("atime: %s\n", atime);
        printf("ctime: %s\n", ctime);
        printf("mtime: %s\n", mtime);
        printf("nlinks: %d\n", stat.nlinks);
        printf("tamEnBytesLog= %d\n", stat.tamEnBytesLog);
        printf("numBloquesOcupados= %d\n\n", stat.numBloquesOcupados);

        bumount();
    }
}
