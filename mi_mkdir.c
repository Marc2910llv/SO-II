// Pere Joan Vives Morey
// Marc Llobera Villalonga
// Carlos Lozano Alemañy

#include "directorios.h"

int main(int argc, char const *argv[])
{
    if (argc != 4)
    {
        perror("ERROR DE SINTAXIS EN mi_mkdir.c");
        return -1;
    }

    if (atoi(argv[2]) < 0 || atoi(argv[2]) > 7)
    {
        perror("ERROR EN mi_mkdir.c, LOS PERMISOS NO SON CORRECTOS");
        return -1;
    }

    if ((argv[3][strlen(argv[3]) - 1] == '/'))
    {
        if (bmount(argv[1]) == -1)
        {
            perror("ERROR EN mi_mkdir.c AL INTENTAR MONTAR EL DISCO EN EL SISTEMA");
            return -1;
        }
        int error= mi_creat(argv[3], atoi(argv[2]);
        if (error < 0)
        {
            mostrar_error_buscar_entrada(error);
            return -1;
        }
        if(bumount() == -1){
            perror("ERROR EN mi_mkfs.c AL INTENTAR CERRAR EL DIRECTORIO");
        }
    }
    else
    {
        if (bmount(argv[1]) == -1)
        {
            perror("ERROR EN mi_mkdir.c AL INTENTAR MONTAR EL DISCO EN EL SISTEMA");
            return -1;
        }
        int error = mi_creat(argv[3], atoi(argv[2]);
        if (error < 0)
        {
            mostrar_error_buscar_entrada(error);
            return -1;
        }
        if(bumount() == -1){
            perror("ERROR EN mi_mkfs.c AL INTENTAR CERRAR EL FICHERO");
        }
    }
    return 0;
}