// Pere Joan Vives Morey
// Marc Llobera Villalonga
// Carlos Lozano Alemañy
#include "directorios.h"

int main(int argc, char const *argv[])
{

    if (argc != 4)
    {
        perror("Error de sintaxis: ./mi_mkdir <disco><permisos></ruta>\n");
        return -1;
    }

    if (atoi(argv[2]) < 0 || atoi(argv[2]) > 7)
    {
        perror("mi_touch.c: Permisos no válidos.\n");
    }

    unsigned char permisos = atoi(argv[2]);

    if ((argv[3][strlen(argv[3]) - 1] != '/'))
    {

        if (bmount(argv[1]) == -1)
        {
            return -1;
        }
        int error;
        if ((error = mi_creat(argv[3], permisos)) < 0)
        {
            mostrar_error_buscar_entrada(error);
            return -1;
        }
        bumount();
    }
    else
    {
        perror("No es una ruta de fichero válida\n");
    }
    return 0;
}