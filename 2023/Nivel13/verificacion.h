/*
 *   Marc Llobera Villalonga
 */

// verificacion.h

#include "simulacion.h"

struct INFORMACION
{
    int pid;
    unsigned int nEscrituras; // validadas 1
    struct REGISTRO PrimeraEscritura;
    struct REGISTRO UltimaEscritura;
    struct REGISTRO MenorPosicion;
    struct REGISTRO MayorPosicion;
};