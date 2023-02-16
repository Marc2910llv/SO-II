// Pere Joan Vives Morey
// Marc Llobera Villalonga
// Carlos Lozano Alemañy
#include "ficheros.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    int nbytes, ninodo;
    // struct inodo inodo;

    if (argc != 4)
    {
        printf("Sintaxis correcta: truncar <nombre_dispositivo> <ninodo> <nbytes>\n");
        return -1;
    }

    nbytes = atoi(argv[3]);
    ninodo = atoi(argv[2]);

    if (bmount(argv[1]) == -1)
    {
        perror("Error a l'hora de montar el sistema\n");
        return -1;
    }

    if (nbytes == 0)
    {
        if (liberar_inodo(ninodo) == -1)
        {
            perror("ERROR EN truncar.c AL INTENTAR LIBERAR UN INODO");
            return -1;
        }
    }
    else
    {
        mi_truncar_f(ninodo, nbytes);
    }

    struct STAT stat;
    // leer_inodo(ninodo, &inodo);
    if (mi_stat_f(ninodo, &stat) == -1)
    {
        perror("ERROR EN truncar.c AL INTENTAR OBTENER LA INFORMACIÓN DEL INODO");
        return -1;
    }

    // Variables utilizadas para cambiar el formato de la fecha y hora.
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

    // Información del inodo escrito
    printf("\nDATOS INODO %d:\n", ninodo);
    printf("tipo=%c\n", stat.tipo);
    printf("permisos=%d\n", stat.permisos);
    printf("atime: %s\n", atime);
    printf("ctime: %s\n", ctime);
    printf("mtime: %s\n", mtime);
    printf("nLinks= %d\n", stat.nlinks);
    printf("tamEnBytesLog= %d\n", stat.tamEnBytesLog);
    printf("numBloquesOcupados= %d\n", stat.numBloquesOcupados);

    if (bumount() == -1)
    {
        perror("ERROR EN truncar.c AL INTENTAR CERRAR EL FICHERO");
        return -1;
    }
}
