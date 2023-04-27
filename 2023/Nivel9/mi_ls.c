/*
 *   Marc Llobera Villalonga
 */

#include "directorios.h"

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, RED "Sintaxis: ./mi_ls <disco> </ruta_directorio>\n" RESET);
        return FALLO;
    }

    if (bmount(argv[1]) == FALLO)
    {
        perror("Error main bmount");
        return FALLO;
    }

    char buffer[TAMBUFFER];
    memset(buffer, 0, TAMBUFFER);

    char tipo = 'f';
    if ((argv[2][strlen(argv[2]) - 1] == '/')) // si no es un fichero
    {
        tipo = 'd';
    }

    int total = mi_dir(argv[2], buffer, tipo);
    if (total < FALLO)
    {
        mostrar_error_buscar_entrada(total);
        return FALLO;
    }
    else if (total == FALLO)
    {
        perror("Error main mi_dir");
        return FALLO;
    }

    printf("Total: %d\n", total);
    printf("Tipo\tModo\tmTime\t\t\tTamaño\tNombre\n");
    printf("--------------------------------------------------------------------------------------------\n");
    printf("%s\n", buffer);

    if (bumount() == FALLO)
    {
        perror("Error main bumount");
    }
}