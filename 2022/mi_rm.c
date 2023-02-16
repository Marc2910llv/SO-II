// Pere Joan Vives Morey
// Marc Llobera Villalonga
// Carlos Lozano Alemañy
#include "directorios.h"

int main(int argc, char *argv[])
{

    if (argc != 3)
    {
        fprintf(stderr, "Sintaxis: ./mi_rm <disco> </ruta_fichero>\n");
        return -1;
    }

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