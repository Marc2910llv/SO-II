//Pere Joan Vives Morey
//Marc Llobera Villalonga
//Carlos Lozano Alemañy

//verificacion.h
#include "simulacion.h"
 
struct INFORMACION {
  int pid;
  unsigned int nEscrituras; //validadas 
  struct REGISTRO PrimeraEscritura;
  struct REGISTRO UltimaEscritura;
  struct REGISTRO MenorPosicion;
  struct REGISTRO MayorPosicion;
};