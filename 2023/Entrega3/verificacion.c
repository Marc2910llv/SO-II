/*
 *   Marc Llobera Villalonga
 */

#include "verificacion.h"

int main(int argc, char const *argv[])
{
    if (argc != 3)
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

    printf("dir_sim: %s\n", argv[2]);
    printf("numentradas: %d NUMPROCESOS: %d\n", numentradas, NUMPROCESOS); // 100S

    char nfichero[128];
    sprintf(nfichero, "%s%s", argv[2], "informe.txt");
    if (mi_creat(nfichero, 7) < 0)
    {
        perror("ERROR EN verificacion.c, EL NÚMERO DE ENTRADAS NO ES CORRECTO");
        bumount();
        exit(0);
    }

    struct entrada entradas[numentradas];
    int error = mi_read(argv[2], entradas, 0, sizeof(entradas));
    if (error == FALLO)
    {
        perror("Error main mi_read");
        return FALLO;
    }
    else if (error < FALLO)
    {
        mostrar_error_buscar_entrada(error);
        return FALLO;
    }

    int nbytes = 0;
    for (int nentrada = 0; nentrada < numentradas; nentrada++)
    {
        struct INFORMACION info;
        pid_t pid = atoi(strchr(entradas[nentrada].nombre, '_') + 1);
        info.pid = pid;
        info.nEscrituras = 0;

        char prueba[128];
        sprintf(prueba, "%s%s/prueba.dat", argv[2], entradas[nentrada].nombre);

        int offset = 0;
        int cant_registros_buffer_escrituras = 256;
        struct REGISTRO buffer_escrituras[cant_registros_buffer_escrituras];
        memset(buffer_escrituras, 0, sizeof(buffer_escrituras));
        while (mi_read(prueba, buffer_escrituras, offset, sizeof(buffer_escrituras)) > 0)
        {
            for (int n = 0; n < cant_registros_buffer_escrituras; n++)
            {
                if (buffer_escrituras[n].pid == info.pid)
                {
                    if (info.nEscrituras == 0)
                    {
                        info.MayorPosicion = buffer_escrituras[n];
                        info.MenorPosicion = buffer_escrituras[n];
                        info.PrimeraEscritura = buffer_escrituras[n];
                        info.UltimaEscritura = buffer_escrituras[n];
                        // ya será la de menor posición puesto que hacemos un barrido secuencial
                    }
                    else
                    {
                        if ((difftime(buffer_escrituras[n].fecha, info.PrimeraEscritura.fecha)) <= 0 &&
                            buffer_escrituras[n].nEscritura < info.PrimeraEscritura.nEscritura)
                        {
                            info.PrimeraEscritura = buffer_escrituras[n];
                        }
                        if ((difftime(buffer_escrituras[n].fecha, info.UltimaEscritura.fecha)) >= 0 &&
                            buffer_escrituras[n].nEscritura > info.UltimaEscritura.nEscritura)
                        {
                            info.UltimaEscritura = buffer_escrituras[n];
                        }
                        if (buffer_escrituras[n].nRegistro < info.MenorPosicion.nRegistro)
                        {
                            info.MenorPosicion = buffer_escrituras[n];
                        }
                        if (buffer_escrituras[n].nRegistro > info.MayorPosicion.nRegistro)
                        {
                            info.MayorPosicion = buffer_escrituras[n];
                        }
                    }
                    info.nEscrituras++;
                }
            }
            memset(&buffer_escrituras, 0, sizeof(buffer_escrituras));
            offset += sizeof(buffer_escrituras);
        }
        printf("[%d) %d escrituras validadas en %s]\n", nentrada + 1, NUMESCRITURAS, prueba);

        struct tm *tm;
        char buffer[BLOCKSIZE];
        memset(buffer, 0, BLOCKSIZE);

        sprintf(buffer, "PID: %i\nNumero de escrituras: %i\n", pid, info.nEscrituras);

        tm = localtime(&info.PrimeraEscritura.fecha);
        sprintf(buffer + strlen(buffer), "%s %i %i %s",
                "Primera escritura",
                info.PrimeraEscritura.nEscritura,
                info.PrimeraEscritura.nRegistro,
                asctime(tm));

        tm = localtime(&info.UltimaEscritura.fecha);
        sprintf(buffer + strlen(buffer), "%s %i %i %s",
                "Ultima escritura",
                info.UltimaEscritura.nEscritura,
                info.UltimaEscritura.nRegistro,
                asctime(tm));

        tm = localtime(&info.MenorPosicion.fecha);
        sprintf(buffer + strlen(buffer), "%s %i %i %s",
                "Menor posicion",
                info.MenorPosicion.nEscritura,
                info.MenorPosicion.nRegistro,
                asctime(tm));

        tm = localtime(&info.MayorPosicion.fecha);
        sprintf(buffer + strlen(buffer), "%s %i %i %s\n\n",
                "Mayor posicion",
                info.MayorPosicion.nEscritura,
                info.MayorPosicion.nRegistro,
                asctime(tm));

        if ((nbytes += mi_write(nfichero, &buffer, nbytes, strlen(buffer))) < EXITO)
        {
            perror("Error main mi_write");
            bumount();
            return FALLO;
        }
    }
    if (bumount() == FALLO)
    {
        perror("Error main bumount");
        return FALLO;
    }
}