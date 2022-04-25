#include "ficheros.h"

int main(int argc, char *argv[])
{

    int ninodo = atoi(argv[2]);
    int permisos = atoi(argv[3]);
    if (argc != 4)
    {
        printf("Sintaxis: permitir <nombre_dispositivo> <ninodo> <permisos>\n");
        return -1;
    }

    if (bmount(argv[1]) == -1)
    {
        perror("Error a l'hora de montar el sistema\n");
        return -1;
    }

    if (mi_chmod_f(ninodo, permisos))
    {
        perror("Error amb el canvi de permisos\n");
        return -1;
    }

    bumount();
    return -1;
}