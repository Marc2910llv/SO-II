#include <string.h>
#include "ficheros.h"
#include <time.h> 

#define tambuffer 1500

int main(int argc, char *argv[]){
    int bytes = tambuffer;
    char buffer[bytes];
    int nombre;
    int bytesllegits,offset,cantidad;
    offset=0;
    struct inodo inodo;
    struct superbloque SB;
    if(argc<3){
        printf("Sintaxis : leer <nombre_dispositivo> <ninodo>\n ");
    }else{
     memset(buffer,0,bytes);
     nombre=atoi(argv[2]); 

     bmount(argv[1]);   
     
    if (bread(0, &SB) == -1)
    {
        perror("ERROR EN leer_sf AL LEER EL SUPERBLOQUE");
        return -1;
    }
     cantidad = mi_read_f(nombre,buffer,offset,bytes);

     while(cantidad>0){
         bytesllegits = bytesllegits+cantidad;
         write(1,buffer,cantidad);
         memset(buffer,0,bytes);
         offset = bytes+offset;
         cantidad = mi_read_f(nombre,buffer,offset,bytes);

     }
     leer_inodo(nombre,&inodo);
     fprintf(stderr, "total_bytesleidos: %d\ntamEnBytesLog: %d\n", bytesllegits, inodo.tamEnBytesLog);
     bumount();
     return 1;
    }

}