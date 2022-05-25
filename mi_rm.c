// Pere Joan Vives Morey
// Marc Llobera Villalonga
// Carlos Lozano Alemañy
#include "directorios.h"

int main(int argc, char **argv)
{
    //Comprobamos que los parametros sean correctos
    if (argc != 3)
    {
        fprintf(stderr, "Sintaxis: ./mi_rm <disco> </ruta_fichero>\n");
        return -1;
    }
    // Comprueba si es un directorio
/*     if ((argv[2][strlen(argv[2]) - 1] == '/'))
    {
        fprintf(stderr, "Error: La ruta no es un fichero.\n");
        return FAILURE;
    } */
    // Monta el dispositivo virtual en el sistema.
    if (bmount(argv[1]) == -1)
    {
        fprintf(stderr, "Error: al montar el dispositivo.\n");
        return -1;
    }

    if (mi_unlink(argv[2]) < 0)
    {
        return -1;
    }
    bumount();
    return 0;
}