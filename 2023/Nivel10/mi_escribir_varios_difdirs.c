//Programa para testear las cachés de directorios > 1
#include "directorios.h"
#define DIFDIRS 2  //Se crearán tantos directorios como indique ese valor y dentro de cada uno también


int main(int argc, char **argv){

   char camino[256];
   memset(camino, 0 , sizeof(camino));
   unsigned int offset=0;
   unsigned int escritos=0;

  //Comprobamos sintaxis
  if (argc!=3) {
    fprintf(stderr, "Sintaxis: mi_escribir_varios_difdirs <nombre_dispositivo> <texto> \n");
    exit(-1);
   }

   //montamos el dispositivo
   if(bmount(argv[1])<0) return -1;
   //obtenemos el texto y su longitud
   char *buffer_texto = argv[2];
   int longitud=strlen(buffer_texto);

   //creación de subdirectorios y ficheros
   for (int i=0; i<DIFDIRS; i++){
      sprintf (camino, "/dir%d/", i);
      if (mi_creat (camino, 6) < 0) {
         fprintf (stderr, "Error al crear directorio %s\n", camino);
		  	bumount();
		  	exit(EXIT_FAILURE);
		} else 
         fprintf(stderr,"[mi_escribir_varios_difdirs.c → Creado directorio %s]\n", camino);
      for (int j=0; j<DIFDIRS; j++){
         sprintf (camino, "/dir%d/dir%d_%d/", i, i, j);
         if (mi_creat (camino, 6) < 0) {
          		fprintf (stderr, "Error al crear directorio %s\n", camino);
		  	  	bumount();
		  		exit(EXIT_FAILURE);
		  	} else fprintf(stderr,"[mi_escribir_varios_difdirs.c → Creado directorio %s]\n", camino);

         strcat(camino, "fichero.txt");
         if (mi_creat (camino, 6) < 0) {
          	fprintf (stderr, "Error al crear fichero %s\n", camino);
		  	  	bumount();
		  		exit(EXIT_FAILURE);
		  	} else fprintf(stderr,"[mi_escribir_varios_difdirs.c → Creado directorio %s]\n", camino);

         memset(camino,0,sizeof(camino));
      }
      memset(camino,0,sizeof(camino));
   }

   //escrituras
   srand(time(NULL));
   for (int i=0; i<DIFDIRS*100; i++){
      int num1 = rand()%DIFDIRS;
      int num2 = rand()%DIFDIRS;
      sprintf (camino, "/dir%d/dir%d_%d/fichero.txt", num1, num1, num2);
      escritos += mi_write(camino,buffer_texto,offset,longitud); 
      fprintf(stderr,"[mi_escribir_varios_difdirs.c → Bytes escritos en %s: %d]\n", camino, escritos);
      escritos = 0;
   }
   bumount();
}
