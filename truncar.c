#include "ficheros.h"

int main(int argc, char *argv[])
{
    int nbytes, ninodo;
    struct inodo inodo;

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
        leer_inodo(ninodo, &inodo);

        printf("DATOS INODO %d\n", ninodo);
        printf("tipo= %c\n", inodo.tipo);
        printf("permisos= %d\n", inodo.permisos);
        printf("atime: %ld\n", inodo.atime);
        printf("ctime: %ld\n", inodo.ctime);
        printf("mtime: %ld\n", inodo.mtime);
        printf("nlinks: %d\n", inodo.nlinks);
        printf("tamEnBytesLog= %d\n", inodo.tamEnBytesLog);
        printf("numBloquesOcupados= %d\n\n", inodo.numBloquesOcupados);

        bumount();
    }
}
