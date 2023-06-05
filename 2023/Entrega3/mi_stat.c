/*
 *   Marc Llobera Villalonga
 */

#include "directorios.h"

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, RED "Sintaxis: ./mi_stat <disco> </ruta>\n" RESET);
        return FALLO;
    }

    if (bmount(argv[1]) == FALLO)
    {
        perror("Error main bmount");
        return FALLO;
    }

    struct STAT stat;

    int error = mi_stat(argv[2], &stat);
    if (error < 0)
    {
        mostrar_error_buscar_entrada(error);
        return FALLO;
    }

    char atime[80];
    char ctime[80];
    char mtime[80];

    struct tm *tm;

    tm = localtime(&stat.atime);
    strftime(atime, 80, "%a %Y-%m-%d %H:%M:%S", tm);
    tm = localtime(&stat.ctime);
    strftime(ctime, 80, "%a %Y-%m-%d %H:%M:%S", tm);
    tm = localtime(&stat.mtime);
    strftime(mtime, 80, "%a %Y-%m-%d %H:%M:%S", tm);

    printf("DATOS INODO %d\n", error);
    printf("tipo= %c\n", stat.tipo);
    printf("permisos= %d\n", stat.permisos);
    printf("atime: %s\n", atime);
    printf("ctime: %s\n", ctime);
    printf("mtime: %s\n", mtime);
    printf("nlinks: %d\n", stat.nlinks);
    printf("tamEnBytesLog= %d\n", stat.tamEnBytesLog);
    printf("numBloquesOcupados= %d\n\n", stat.numBloquesOcupados);

    if (bumount() == FALLO)
    {
        perror("Error main bumount");
    }
}