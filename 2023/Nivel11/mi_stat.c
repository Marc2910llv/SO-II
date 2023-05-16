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

    time_t a, b, c;
    struct tm *t1, *t2, *t3;
    a = stat.atime;
    b = stat.ctime;
    c = stat.mtime;

    t1 = localtime(&a);
    t2 = localtime(&b);
    t3 = localtime(&c);

    strftime(atime, 80, "%a %Y-%m-%d %H:%M:%S", t1);
    strftime(ctime, 80, "%a %Y-%m-%d %H:%M:%S", t2);
    strftime(mtime, 80, "%a %Y-%m-%d %H:%M:%S", t3);

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