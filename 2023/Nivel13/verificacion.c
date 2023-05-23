/*
 *   Marc Llobera Villalonga
 */

#include "verificacion.h"

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, RED "Sintaxis: ./verificacion <nombre_dispositivo> <directorio_simulación>\n" RESET);
        return FALLO;
    }

    if (bmount(argv[1]) == FALLO)
    {
        perror("Error main bmount");
        return FALLO;
    }

    struct STAT p_stat;
    mi_stat(argv[2], &p_stat);
    printf("dir_sim: %s\n", argv[2]);
    printf("numentradas: %d NUMPROCESOS: %d\n", NUMESCRITURAS, NUMPROCESOS); // 100

    int numentradas = (p_stat.tamEnBytesLog / sizeof(struct entrada));
    if (numentradas != NUMPROCESOS)
    {
        perror("Error main numentradas != NUMPROCESOS");
        if (bumount() == FALLO)
        {
            perror("Error main bumount");
            return FALLO;
        }
        return FALLO;
    }

        char nfichero[500];
    sprintf(nfichero, "%s%s", argv[2], "informe.txt");
    if (mi_creat(nfichero, 7) < 0)
    {
        perror("ERROR EN verificacion.c, EL NÚMERO DE ENTRADAS NO ES CORRECTO");
        bumount();
        exit(0);
    }
    //-Leerlosdirectorioscorrespondientesalosprocesos. //Entradas del directorio de simulación
    //-Paracadaentradadedirectoriodeunprocesohacer
    //-Leerlaentradadedirectorio.2
    //-ExtraerelPIDapartirdelnombredelaentradayguardarloenelregistroinfo.3
    // Recorrer secuencialmente el fichero prueba.dat utilizando buffer de N registros de escrituras: 4
    int cant_registros_buffer_escrituras = 256;
    struct REGISTRO buffer_escrituras[cant_registros_buffer_escrituras];
    memset(buffer_escrituras, 0, sizeof(buffer_escrituras));
    while (mi_read(prueba, buffer_escrituras, offset, sizeof(buffer_escrituras)) > 0)
    {
    }
    // Mientrashayaescriturasenprueba.dathacer5
    // Leerunaescritura.
    // Silaescrituraesválida entonces6
    // Sieslaprimeraescrituravalidadaentonces
    // Inicializarlosregistrossignificativosconlosdatosdeesaescritura.
    // ya será la de menor posición puesto que hacemos un barrido secuencial
    // sino
    // Compararnºdeescritura(paraobtenerprimerayúltima)yactualizarlassiespreciso
    // fsi
    // Incrementarcontadorescriturasvalidadas.
    // fmientras
    // Obtenerlaescrituradelaúltimaposición.
    // Añadirlainformacióndelstructinfoalficheroinforme.txtporelfinal.
    // fpara
    if (bumount() == FALLO)
    {
        perror("Error main bumount");
        return FALLO;
    }
}