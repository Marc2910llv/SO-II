// Pere Joan Vives Morey
// Marc Llobera Villalonga
// Carlos Lozano Alemañy
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

  if (bmount(argv[1]) == -1)
  {
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

  printf("\nsizeof struct superbloque: %ld\n", sizeof(struct superbloque));
  printf("sizeof struct inodo:  %ld\n", sizeof(struct inodo));

  printf("\nRECORRIDO LISTA ENLAZADA DE INODOS LIBRES\n");
  // Podéis hacer también un recorrido de la lista de inodos libres (mostrando para cada inodo el campo punterosDirectos[0]).
  struct inodo inodos[BLOCKSIZE / INODOSIZE];
  int contlibres = 0;

  for (int i = SB.posPrimerBloqueAI; i <= SB.posUltimoBloqueAI; i++)
  {
    //&inodos
    if (bread(i, inodos) == -1)
    {
      return -1;
    }

    for (int j = 0; j < BLOCKSIZE / INODOSIZE; j++)
    {
      if ((inodos[j].tipo == 'l'))
      {
        contlibres++;
        if (contlibres < 20)
        {
          printf("%d ", contlibres);
        }
        else if (contlibres == 21)
        {
          printf("... ");
        }
        else if ((contlibres > 24990) && (contlibres < SB.totInodos))
        {
          printf("%d ", contlibres);
        }
        else if (contlibres == SB.totInodos)
        {
          printf("-1 \n");
        }
        contlibres--;
      }
      contlibres++;
    }
  }

  // Probación el tamaño del tipo time_t para vuestra plataforma/compilador:
  printf("\nsizeof time_t is: %ld\n", sizeof(time_t));

  printf("\nRESERVAMOS UN BLOQUE Y LUEGO LO LIBERAMOS:\n");
  int reservado = reservar_bloque(); // Actualiza el SB
  bread(posSB, &SB);                 // Actualizar los valores del SB

  printf("Se ha reservado el bloque físico nº %i que era el 1º libre indicado por el MB.\n", reservado);
  printf("SB.cantBloquesLibres: %i\n", SB.cantBloquesLibres);
  liberar_bloque(reservado);
  bread(posSB, &SB); // Actualizar los valores del SB

  printf("Liberamos ese bloque, y después SB.cantBloquesLibres: %i\n\n", SB.cantBloquesLibres);
  printf("MAPA DE BITS CON BLOQUES DE METADATOS OCUPADOS\n");
  int bit = leer_bit(posSB);
  printf("leer_bit(%i) = %i\n", posSB, bit);
  bit = leer_bit(SB.posPrimerBloqueMB);
  printf("leer_bit(%i) = %i\n", SB.posPrimerBloqueMB, bit);
  bit = leer_bit(SB.posUltimoBloqueMB);
  printf("leer_bit(%i) = %i\n", SB.posUltimoBloqueMB, bit);
  bit = leer_bit(SB.posPrimerBloqueAI);
  printf("leer_bit(%i) = %i\n", SB.posPrimerBloqueAI, bit);
  bit = leer_bit(SB.posUltimoBloqueAI);
  printf("leer_bit(%i) = %i\n", SB.posUltimoBloqueAI, bit);
  bit = leer_bit(SB.posPrimerBloqueDatos);
  printf("leer_bit(%i) = %i\n", SB.posPrimerBloqueDatos, bit);
  bit = leer_bit(SB.posUltimoBloqueDatos);
  printf("leer_bit(%i) = %i\n", SB.posUltimoBloqueDatos, bit);

  printf("\nDATOS DEL DIRECTORIO RAIZ\n\n");
  struct tm *ts;
  char atime[80];
  char mtime[80];
  char ctime[80];
  struct inodo inodo;
  int ninodo = 0; // el directorio raiz es el inodo 0
  leer_inodo(ninodo, &inodo);
  ts = localtime(&inodo.atime);
  strftime(atime, sizeof(atime), "%a %Y-%m-%d %H:%M:%S", ts);
  ts = localtime(&inodo.mtime);
  strftime(mtime, sizeof(mtime), "%a %Y-%m-%d %H:%M:%S", ts);
  ts = localtime(&inodo.ctime);
  strftime(ctime, sizeof(ctime), "%a %Y-%m-%d %H:%M:%S", ts);
  printf("tipo: %c\n", inodo.tipo);
  printf("permisos: %i\n", inodo.permisos);
  printf("ID: %d \nATIME: %s \nMTIME: %s \nCTIME: %s\n", ninodo, atime, mtime, ctime);
  printf("nlinks: %i\n", inodo.nlinks);
  printf("tamaño en bytes lógicos: %i\n", inodo.tamEnBytesLog);
  printf("Número de bloques ocupados: %i\n", inodo.numBloquesOcupados);

  int inodoReservado = reservar_inodo('f', 6);
  bread(posSB, &SB);

  printf("\nINODO %d - TRADUCCION DE LOS BLOQUES LOGICOS 8, 204, 30.004, 400.004 y 468.750\n", inodoReservado);
  traducir_bloque_inodo(inodoReservado, 8, 1);
  traducir_bloque_inodo(inodoReservado, 204, 1);
  traducir_bloque_inodo(inodoReservado, 30004, 1);
  traducir_bloque_inodo(inodoReservado, 400004, 1);
  traducir_bloque_inodo(inodoReservado, 468750, 1);

  printf("\nDATOS DEL INODO RESERVADO: %d\n", inodoReservado);
  leer_inodo(inodoReservado, &inodo); // Leemos el Inodo reservado
  ts = localtime(&inodo.atime);
  strftime(atime, sizeof(atime), "%a %Y-%m-%d %H:%M:%S", ts);
  ts = localtime(&inodo.mtime);
  strftime(mtime, sizeof(mtime), "%a %Y-%m-%d %H:%M:%S", ts);
  ts = localtime(&inodo.ctime);
  strftime(ctime, sizeof(ctime), "%a %Y-%m-%d %H:%M:%S", ts);
  printf("tipo: %c\n", inodo.tipo);
  printf("permisos: %i\n", inodo.permisos);
  printf("ATIME: %s \nMTIME: %s \nCTIME: %s\n", atime, mtime, ctime);
  printf("nlinks: %i\n", inodo.nlinks);
  printf("tamaño en bytes lógicos: %i\n", inodo.tamEnBytesLog);
  printf("Número de bloques ocupados: %i\n", inodo.numBloquesOcupados);
  printf("SB.posPrimerInodoLibre = %d\n", SB.posPrimerInodoLibre);

  // Mostrar creación directorios y errores
  mostrar_buscar_entrada("pruebas/", 1);                 // ERROR_CAMINO_INCORRECTO
  mostrar_buscar_entrada("/pruebas/", 0);                // ERROR_NO_EXISTE_ENTRADA_CONSULTA
  mostrar_buscar_entrada("/pruebas/docs/", 1);           // ERROR_NO_EXISTE_DIRECTORIO_INTERMEDIO
  mostrar_buscar_entrada("/pruebas/", 1);                // creamos /pruebas/
  mostrar_buscar_entrada("/pruebas/docs/", 1);           // creamos /pruebas/docs/
  mostrar_buscar_entrada("/pruebas/docs/doc1", 1);       // creamos /pruebas/docs/doc1
  mostrar_buscar_entrada("/pruebas/docs/doc1/doc11", 1); // ERROR_NO_SE_PUEDE_CREAR_ENTRADA_EN_UN_FICHERO
  mostrar_buscar_entrada("/pruebas/", 1);                // ERROR_ENTRADA_YA_EXISTENTE
  mostrar_buscar_entrada("/pruebas/docs/doc1", 0);       // consultamos /pruebas/docs/doc1
  mostrar_buscar_entrada("/pruebas/docs/doc1", 1);       // creamos /pruebas/docs/doc1
  mostrar_buscar_entrada("/pruebas/casos/", 1);          // creamos /pruebas/casos/
  mostrar_buscar_entrada("/pruebas/docs/doc2", 1);       // creamos /pruebas/docs/doc2

  if (bumount() == -1)
  {
    perror("ERROR EN leer_sf AL INTENTAR CERRAR EL FICHERO");
    return -1;
  }
  return 0;
}

void mostrar_buscar_entrada(char *camino, char reservar)
{
  unsigned int p_inodo_dir = 0;
  unsigned int p_inodo = 0;
  unsigned int p_entrada = 0;
  int error;

  printf("\ncamino: %s, reservar: %d\n", camino, reservar);
  error = buscar_entrada(camino, &p_inodo_dir, &p_inodo, &p_entrada, reservar, 6);
  if (error < 0)
  {
    mostrar_error_buscar_entrada(error);
  }
  printf("**********************************************************************\n");
  return;
}
