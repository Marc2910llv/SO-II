// Pere Joan Vives Morey
// Marc Llobera Villalonga
// Carlos Lozano Alemañy
#include "directorios.h"
#include <string.h>

int extraer_camino(const char *camino, char *inicial, char *final, char *tipo)
{
    char delimitador[] = "/";
    char *token;

    token = strtok(camino, delimitador);
    inicial = token;
    token = strtok(NULL, delimitador);
    if (token != NULL)
    {
        tipo = "d";
        final = "";
        char *buffer;
        while (token != NULL)
        {
            strcat(buffer, "/");
            strcat(buffer, token);
            token = strtok(NULL, delimitador);
        }
        final = buffer;
        return 1;
    }
    else
    {
        tipo = "f";
        final = "";
        return 0;
    }
    perror("ERROR EN extraer_camino AL SEPARAR LA RUTA DEL DIRECTORIO O EL FICHERO");
    return -1;
}


int buscar_entrada(const char *camino_parcial, unsigned int *p_inodo_dir, unsigned int *p_inodo,
                   unsigned int *p_entrada, char reservar, unsigned char permisos)
{
}
