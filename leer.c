//Pere Joan Vives Morey
//Marc Llobera Villalonga
//Carlos Lozano Alemañy
#include <string.h>
#include "ficheros.h"
#include <stdlib.h>

#define tambuffer 4096 //tamaño que vamos a leer

int main(int argc, char const *argv[])
{
    int bytes = tambuffer;
    char buffer[bytes];
    int nombre;
    int bytesllegits, offset;
    bytesllegits=0;
    offset = 0; //empezamos en el offset 0
    struct inodo inodo;
    if (argc < 3) //si no hay almenos 3 argumentos presentamos la sintaxis
    {
        printf("Sintaxis : leer <nombre_dispositivo> <ninodo>\n ");
    }
    else
    {
        memset(buffer, 0, bytes); //vaciamos el buffer 
        nombre = atoi(argv[2]); //recogemos el numero del inodo

        if(bmount(argv[1])==-1){ //abrimos el fichero
            perror("ERROR EN leer.c AL INTENTAR ABRIR EL FICHERO");
            return -1;
        }

        int cantidad = mi_read_f(nombre, buffer, offset, bytes); //leemos una primera cantidad de bytes del inodo

        while (cantidad > 0) //hasta que la cantidad no sea 0 (no lleguemos al final)
        {
            bytesllegits = bytesllegits + cantidad;
            write(1, buffer, cantidad);
            memset(buffer, 0, bytes);
            offset += bytes; //cambiamos el offset
            cantidad = mi_read_f(nombre, buffer, offset, bytes);
            if (cantidad == -1)
            {
                perror("ERROR EN leer.c EN mi_read_f");
                return -1;
            }
        }
        //imprimimos la información
        if(leer_inodo(nombre, &inodo)==-1){
            perror("ERROR EN leer.c AL INTENTAR LEER UN INODO");
            return -1;
        }
        fprintf(stderr, "total_bytesleidos: %d\n", bytesllegits);
        fprintf(stderr, "tamEnBytesLog: %d\n\n", inodo.tamEnBytesLog);

        if(bumount()==-1){
            perror("ERROR EN leer.c AL INTENTAR CERRAR EL FICHERO");
            return -1;
        }
        return 0;
    }
}