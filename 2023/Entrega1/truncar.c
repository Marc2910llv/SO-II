/*
 *   Marc Llobera Villalonga
 */

#include "ficheros.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Sintaxis correcta: truncar <nombre_dispositivo> <ninodo> <nbytes>\n");
        return FALLO;
    }

    if (bmount(argv[1]) == FALLO)
    {
        perror("Error a l'hora de montar el sistema\n");
        return FALLO;
    }

    int ninodo = atoi(argv[2]);
    int nbytes = atoi(argv[3]);

    if (nbytes == 0)
    {
        if (liberar_inodo(ninodo) == FALLO)
        {
            perror("Error main liberar_inodo");
            return FALLO;
        }
    }
    else
    {
        if (mi_truncar_f(ninodo, nbytes) == FALLO)
        {
            perror("Error main mi_truncar_f");
            return FALLO;
        }
    }

    struct STAT stat;
    if (mi_stat_f(ninodo, &stat) == FALLO)
    {
        perror("Error main mi_stat_f");
        return FALLO;
    }

    struct tm *ts;
    char atime[80];
    char mtime[80];
    char ctime[80];
    ts = localtime(&stat.atime);
    strftime(atime, sizeof(atime), "%a %Y-%m-%d %H:%M:%S", ts);
    ts = localtime(&stat.mtime);
    strftime(mtime, sizeof(mtime), "%a %Y-%m-%d %H:%M:%S", ts);
    ts = localtime(&stat.ctime);
    strftime(ctime, sizeof(ctime), "%a %Y-%m-%d %H:%M:%S", ts);

    printf("\nDATOS INODO %d:\n", ninodo);
    printf("tipo=%c\n", stat.tipo);
    printf("permisos=%d\n", stat.permisos);
    printf("atime: %s\n", atime);
    printf("ctime: %s\n", ctime);
    printf("mtime: %s\n", mtime);
    printf("nLinks= %d\n", stat.nlinks);
    printf("tamEnBytesLog= %d\n", stat.tamEnBytesLog);
    printf("numBloquesOcupados= %d\n", stat.numBloquesOcupados);

    if (bumount() == FALLO)
    {
        perror("Error main bumount");
    }
}
