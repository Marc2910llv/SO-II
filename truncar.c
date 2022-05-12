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
            if (liberar_inodo(ninodo) == -1)
            {
                perror("ERROR EN truncar.c AL INTENTAR LIBERAR UN INODO");
                return -1;
            }
        }
        else
        {
            if (mi_truncar_f(ninodo, nbytes) == -1)
            {
                perror("ERROR EN truncar.c AL TRUNCAR");
                return -1;
            }
        }
        struct STAT stat;
        // leer_inodo(ninodo, &inodo);
        if (mi_stat_f(ninodo, &stat) == -1)
        {
            perror("ERROR EN truncar.c AL INTENTAR OBTENER LA INFORMACIÓN DEL INODO");
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
        printf("DATOS INODO %d\n", ninodo);
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
            perror("ERROR EN truncar.c AL INTENTAR CERRAR EL FICHERO");
            return -1;
        }
    }
}
