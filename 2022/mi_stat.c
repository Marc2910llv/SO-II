// Pere Joan Vives Morey
// Marc Llobera Villalonga
// Carlos Lozano Alemañy

#include "directorios.h"

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        perror("ERROR DE SINTAXIS EN mi_stat.c");
        return -1;
    }

    if (bmount(argv[1]) == -1)
    {
        perror("ERROR EN mi_stat.c AL INTENTAR MONTAR EL DISCO");
        return -1;
    }

    
    struct STAT stat;
    int error = mi_stat(argv[2], &stat);
    printf("Entrada: %s\n",argv[2]);
    if (error < 0)
    {
        mostrar_error_buscar_entrada(error);
        return -1;
    }

    // char *t1,*t2,*t3;
    char atime[80]; // arrays que contendrán el tiempo
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

    strftime(atime, 80, "%a %Y-%m-%d %H:%M:%S", t1); // metemos los tiempos en los arrays
    strftime(ctime, 80, "%a %Y-%m-%d %H:%M:%S", t2);
    strftime(mtime, 80, "%a %Y-%m-%d %H:%M:%S", t3);
    // imprimimos la información
    printf("DATOS INODO %d\n", error);
    printf("tipo= %c\n", stat.tipo);
    printf("permisos= %d\n", stat.permisos);
    printf("atime: %s\n", atime);
    printf("ctime: %s\n", ctime);
    printf("mtime: %s\n", mtime);
    printf("nlinks: %d\n", stat.nlinks);
    printf("tamEnBytesLog= %d\n", stat.tamEnBytesLog);
    printf("numBloquesOcupados= %d\n\n", stat.numBloquesOcupados);

    if (bumount() == -1)
    {
        perror("ERROR EN mi_stat.c AL INTENTAR CERRAR");
    }
}