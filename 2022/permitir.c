//Pere Joan Vives Morey
//Marc Llobera Villalonga
//Carlos Lozano Alemañy
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

    if(bumount()==-1){
        perror("ERROR EN permitir.c AL INTENTAR CERRAR EL FICHERO");
        return -1;
    }
    return 0;
}