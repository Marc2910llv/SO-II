#include <string.h>
#include "ficheros.h"
#include <time.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int numero;
    int cantidad;
    int stats;
    int offset[5] = {9000, 209000, 30725000, 409605000, 480000000};
    if (argc != 4)
    {
        printf("Sintaxis: escribir <nombre_dispositivo> <$(cat fichero)> <diferentes_inodos>\n");
        printf("Offsets: 9000, 209000, 30725000, 409605000, 480000000 \n");
        printf("Si diferentes_inodos=0 se reserva un solo inodo para todos los offsets\n");
    }
    else
    {
        printf("Longitud texto: %ld \n", strlen(argv[2]));
        if (bmount(argv[1]) == -1)
        {
            perror("ERROR EN ecribir_c AL INTENTAR ABRIR EL FICHERO");
            return -1;
        }
        numero = reservar_inodo('f', 6);

        for (int i = 0; i < 5; i++)
        {
            printf("Numero inodo reservado: %d \n", numero);
            cantidad = mi_write_f(numero, argv[2], offset[i], strlen(argv[2]));
            printf("Offset: %d \n", offset[i]);
            if (cantidad == -1)
            {
                return -1;
            }
            printf("Bytes escritos: %d \n", cantidad);
            struct STAT stat;
            stats = mi_stat_f(numero, &stat);
            if (stats == -1)
            {
                return -1;
            }

            if (atoi(argv[3]) != 0)
            { // si es diferent de 0, per a cada offset reservarem un inodo diferent
                numero = reservar_inodo('f', 6);
            }

            printf("stat.tamEnBytesLog = %d\n", stat.tamEnBytesLog);
            printf("stat.numBloquesOcupados = %d\n", stat.numBloquesOcupados);
            printf("Problema: %d", stat.tamEnBytesLog);
            printf("\n");
        }

        bumount();
        return 0;
    }
}
