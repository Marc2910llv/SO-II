
#include "directorios.h"

int main(int argc, char *argv[])
{


    if (argc != 5)
    {
        fprintf(stderr, "Sintaxis: mi_escribir <nombre_dispositivo> </ruta_fichero> <texto> <offset>\n");
        exit(-1);
    }

    if (bmount(argv[1]) < 0)
        return -1;

    char *buffer_texto = argv[3];
    int longitud = strlen(buffer_texto);


    if (argv[2][strlen(argv[2]) - 1] == '/')
    {
        fprintf(stderr, "Error: la ruta se corresponde a un directorio");
        exit(-1);
    }
    char *camino = argv[2];

    unsigned int offset = atoi(argv[4]);

    int escritos = 0;
    int varios = 10;
    fprintf(stderr, "longitud texto: %d\n", longitud);
    for (int i = 0; i < varios; i++)
    {
        
        escritos += mi_write(camino, buffer_texto, offset + BLOCKSIZE * i, longitud);
    }
    fprintf(stderr, "Bytes escritos: %d\n", escritos);


    bumount();
}