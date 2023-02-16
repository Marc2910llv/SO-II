//Pere Joan Vives Morey
//Marc Llobera Villalonga
//Carlos Lozano Alemañy
#include <string.h>
#include "ficheros.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int numero; //ninodo
    int cantidad; //cantidad de bytes escritos
    int offset[5] = {9000, 209000, 30725000, 409605000, 480000000}; //offsets
    if (argc != 4) 
    {
        printf("Sintaxis: escribir <nombre_dispositivo> <$(cat fichero)> <diferentes_inodos>\n");
        printf("Offsets: 9000, 209000, 30725000, 409605000, 480000000 \n");
        printf("Si diferentes_inodos=0 se reserva un solo inodo para todos los offsets\n");
    }
    else
    {
        printf("Longitud texto: %ld \n\n", strlen(argv[2])); //longitud del texto
        if (bmount(argv[1]) == -1) //abrimos el archivo
        {
            perror("ERROR EN ecribir_c AL INTENTAR ABRIR EL FICHERO");
            return -1;
        }
        numero = reservar_inodo('f', 6);
        if(numero==-1){
            perror("ERROR EN escribir_c AL INTENTAR RESERVAR UN INODO");
            return -1;
        }

        for (int i = 0; i < 5; i++) //bucle para cada offset;
        {
            printf("Numero inodo reservado: %d \n", numero);
            cantidad = mi_write_f(numero, argv[2], offset[i], strlen(argv[2]));
            printf("Offset: %d \n", offset[i]);
            if (cantidad == -1)
            {
                perror("ERROR EN escribir_c EN mi_write_f");
                return -1;
            }

            printf("Bytes escritos: %d \n", cantidad);
            struct STAT stat;
            if (mi_stat_f(numero, &stat) == -1)
            {
                perror("ERROR EN escribir_c AL INTENTAR mi_stat_f");
                return -1;
            }

            if (atoi(argv[3]) != 0) 
            { // si es diferent de 0, per a cada offset reservarem un inodo diferent
                numero = reservar_inodo('f', 6);
            }

            printf("stat.tamEnBytesLog = %d\n", stat.tamEnBytesLog);
            printf("stat.numBloquesOcupados = %d\n", stat.numBloquesOcupados);
            printf("\n");
        }

        if(bumount()==-1){
            perror("ERROR EN escribir_c AL INTENTAR CERRAR EL FICHERO");
            return -1;
        }
        
        return 0;
    }
}
