//Pere Joan Vives Morey
//Marc Llobera Villalonga
//Carlos Lozano Alemañy
#include <string.h>
//#include "ficheros_basico.h"
#include "directorios.h"
#include <time.h> //esta librería incluirla en ficheros_basico.h

void mostrar_buscar_entrada(char *camino, char reservar);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        perror("ERROR EN leer_sf, SINTAXIS INCORRECTA");
        return -1;
    }

    if(bmount(argv[1])==-1){
        perror("ERROR EN leer_sf AL ABRIR EL FICHERO");
        return -1;
    }
    
    struct superbloque SB;
    if (bread(0, &SB) == -1)
    {
        perror("ERROR EN leer_sf AL LEER EL SUPERBLOQUE");
        return -1;
    }

    printf("DATOS DEL SUPERBLOQUE\n");
    printf("posPrimerBloqueMB = %d\n", SB.posPrimerBloqueMB);
    printf("posUltimoBloqueMB = %d\n", SB.posUltimoBloqueMB);
    printf("posPrimerBloqueAI = %d\n", SB.posPrimerBloqueAI);
    printf("posUltimoBloqueAi = %d\n", SB.posUltimoBloqueAI);
    printf("posPrimerBloqueDatos = %d\n", SB.posPrimerBloqueDatos);
    printf("posUltimoBloqueDatos = %d\n", SB.posUltimoBloqueDatos);
    printf("posInodoRaiz = %d\n", SB.posInodoRaiz);
    printf("posPrimerInodoLibre = %d\n", SB.posPrimerInodoLibre);
    printf("cantBloquesLibres = %d\n", SB.cantBloquesLibres);
    printf("cantInodosLibres = %d\n", SB.cantInodosLibres);
    printf("totBloques = %d\n", SB.totBloques);
    printf("totInodos = %d\n", SB.totInodos);
    //printf("Sizeof struct superbloque = %ld\n", sizeof(SB));
    //printf("Sizeof struct Inodo = %ld\n", sizeof(struct inodo));

    /*
    printf("RESERVAMOS UN BLOQUE Y LUEGO LO LIBERAMOS \n");
    bloque_reservado = reservar_bloque();
    if (bread(0, &SB); == -1)
    {
        perror("ERROR EN leer_sf AL LEER EL SUPERBLOQUE");
        return -1;
    }
    printf("Se ha reservado el bloque físico %d que era el primero libre \n", bloque_reservado);
    printf("cantBloquesLibres = %d\n", SB.cantBloquesLibres);

    printf("MAPA DE BITS CON BLOQUES DE METADATOS OCUPADOS \n");
    int valor = leer_bit(0);
    printf("leer_bit(0): %i \n", valor);
    valor = leer_bit(1);
    printf("leer_bit(1): %i \n", valor);
    valor = leer_bit(13);
    printf("leer_bit(13): %i \n", valor);
    valor = leer_bit(14);
    printf("leer_bit(14): %i \n", valor);
    valor = leer_bit(3138);
    printf("leer_bit(3138): %i \n", valor);
    valor = leer_bit(3139);
    printf("leer_bit(3139): %i \n", valor);
    valor = leer_bit(99999);
    printf("leer_bit(99999): %i \n", valor);
    */


  mostrar_buscar_entrada("pruebas/", 1); //ERROR_CAMINO_INCORRECTO
  mostrar_buscar_entrada("/pruebas/", 0); //ERROR_NO_EXISTE_ENTRADA_CONSULTA
  mostrar_buscar_entrada("/pruebas/docs/", 1); //ERROR_NO_EXISTE_DIRECTORIO_INTERMEDIO
  mostrar_buscar_entrada("/pruebas/", 1); // creamos /pruebas/
  mostrar_buscar_entrada("/pruebas/docs/", 1); //creamos /pruebas/docs/
  mostrar_buscar_entrada("/pruebas/docs/doc1", 1); //creamos /pruebas/docs/doc1
  mostrar_buscar_entrada("/pruebas/docs/doc1/doc11", 1);  
  //ERROR_NO_SE_PUEDE_CREAR_ENTRADA_EN_UN_FICHERO
  mostrar_buscar_entrada("/pruebas/", 1); //ERROR_ENTRADA_YA_EXISTENTE
  mostrar_buscar_entrada("/pruebas/docs/doc1", 0); //consultamos /pruebas/docs/doc1
  mostrar_buscar_entrada("/pruebas/docs/doc1", 1); //creamos /pruebas/docs/doc1
  mostrar_buscar_entrada("/pruebas/casos/", 1); //creamos /pruebas/casos/
  mostrar_buscar_entrada("/pruebas/docs/doc2", 1); //creamos /pruebas/docs/doc2
  
    if (bumount() == -1)
    {
        perror("ERROR EN leer_sf AL INTENTAR CERRAR EL FICHERO");
        return -1;
    }
    return 0;
}

void mostrar_buscar_entrada(char *camino,char reservar){
  unsigned int p_inodo_dir = 0;
  unsigned int p_inodo = 0;
  unsigned int p_entrada = 0;
  int error;

  printf("\ncamino: %s, reservar: %d\n", camino, reservar);
  error = buscar_entrada(camino, &p_inodo_dir, &p_inodo, &p_entrada, reservar, 6);
  if (error < 0) {
    mostrar_error_buscar_entrada(error);
    }
  printf("**********************************************************************\n");  
  return;
}
