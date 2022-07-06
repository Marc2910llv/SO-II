// Pere Joan Vives Morey
// Marc Llobera Villalonga
// Carlos Lozano Alemañy

#include "verificacion.h"

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        perror("ERROR EN verificacion.c, LA SINTAXIS NO ES CORRECTA");
        return -1;
    }

    if (bmount(argv[1]) == -1)
    {
        perror("ERROR EN verificacion.c, AL MONTAR EL DISPOSITIVO");
        return -1;
    }

    struct STAT p_stat;
    mi_stat(argv[2], &p_stat);
    printf("dir_sim: %s\n",argv[2]);
    printf("numentradas: %d NUMPROCESOS: %d\n",ESCRITURAS,PROCESOS);

    int numentradas = (p_stat.tamEnBytesLog / sizeof(struct entrada));
    if (numentradas != PROCESOS)
    {
        perror("ERROR EN verificacion.c, EL NÚMERO DE ENTRADAS NO ES CORRECTO");
        bumount();
        return -1;
    }

    char nfichero[500];
    sprintf(nfichero, "%s%s", argv[2], "informe.txt");
    if (mi_creat(nfichero, 7) < 0)
    {
        perror("ERROR EN verificacion.c, EL NÚMERO DE ENTRADAS NO ES CORRECTO");
        bumount(argv[1]);
        exit(0);
    }

    struct entrada entradas[numentradas];
    int error = mi_read(argv[2], entradas, 0, sizeof(entradas));
    if (error < 0)
    {
        mostrar_error_buscar_entrada(error);
        return -1;
    }

    int nbytes = 0;
    for (int nentrada = 0; nentrada < numentradas; nentrada++) // para cada entrada de directorio de un proceso
    {
        pid_t pid = atoi(strchr(entradas[nentrada].nombre, '_') + 1); // sacamos el pid a traves del nombre
        struct INFORMACION info;
        info.pid = pid;
        info.nEscrituras = 0;

        char prueba[128];
        sprintf(prueba, "%s%s/prueba.dat", argv[2], entradas[nentrada].nombre);

        int cant_registros_buffer_escrituras = 256*40;
        struct REGISTRO buffer_escrituras[cant_registros_buffer_escrituras];
        memset(buffer_escrituras, 0, sizeof(buffer_escrituras));

        int offset = 0;
        // Mientras haya escrituras en prueba.dat hacer
        while (mi_read(prueba, buffer_escrituras, offset, sizeof(buffer_escrituras)) > 0)
        {
            int nregistro = 0;
            while (nregistro < cant_registros_buffer_escrituras)
            {
                // si la escritura es válida entonces:
                if (buffer_escrituras[nregistro].pid == info.pid)
                {
                    if (!info.nEscrituras)
                    { // si es la 1ra
                        // Inicializar los registros significativos con los datos de esa escritura
                        info.MenorPosicion = buffer_escrituras[nregistro];
                        info.MayorPosicion = buffer_escrituras[nregistro];
                        info.PrimeraEscritura = buffer_escrituras[nregistro];
                        info.UltimaEscritura = buffer_escrituras[nregistro];
                        info.nEscrituras++;
                    }
                    else
                    { // obtener 1ra y últ. num escritura y actualizar si cabe
                        if ((difftime(buffer_escrituras[nregistro].fecha, info.PrimeraEscritura.fecha)) <= 0 &&
                            buffer_escrituras[nregistro].nEscritura < info.PrimeraEscritura.nEscritura)
                        {
                            info.PrimeraEscritura = buffer_escrituras[nregistro];
                        }
                        if ((difftime(buffer_escrituras[nregistro].fecha, info.UltimaEscritura.fecha)) >= 0 &&
                            buffer_escrituras[nregistro].nEscritura > info.UltimaEscritura.nEscritura)
                        {
                            info.UltimaEscritura = buffer_escrituras[nregistro];
                        }
                        if (buffer_escrituras[nregistro].nRegistro < info.MenorPosicion.nRegistro)
                        {
                            info.MenorPosicion = buffer_escrituras[nregistro];
                        }
                        if (buffer_escrituras[nregistro].nRegistro > info.MayorPosicion.nRegistro)
                        {
                            info.MayorPosicion = buffer_escrituras[nregistro];
                        }
                        info.nEscrituras++;
                    }
                }
                nregistro++;
            }
            memset(&buffer_escrituras, 0, sizeof(buffer_escrituras));
            offset += sizeof(buffer_escrituras);
        }
        printf("[%d) %d escrituras validadas en %s]\n",nentrada,ESCRITURAS,prueba);

        char tiempoPrimero[100];
        char tiempoUltimo[100];
        char tiempoMenor[100];
        char tiempoMayor[100];
        struct tm *tm;

        char buffer[BLOCKSIZE];
        memset(buffer, 0, BLOCKSIZE);

        tm = localtime(&info.PrimeraEscritura.fecha);
        //strftime(tiempoPrimero, sizeof(tiempoPrimero), "%a %Y-%m-%d %H:%M:%S", tm);
        sprintf(buffer, "PID: %i\nNumero de escrituras: %i\n", pid, info.nEscrituras);
        sprintf(buffer + strlen(buffer), "%s %i %i %s",
                "Primera escritura",
                info.PrimeraEscritura.nEscritura,
                info.PrimeraEscritura.nRegistro,
                asctime(tm));

        tm = localtime(&info.UltimaEscritura.fecha);
        //strftime(tiempoUltimo, sizeof(tiempoUltimo), "%a %Y-%m-%d %H:%M:%S", tm);
        sprintf(buffer + strlen(buffer), "%s %i %i %s",
                "Ultima escritura",
                info.UltimaEscritura.nEscritura,
                info.UltimaEscritura.nRegistro,
                asctime(tm));
        tm = localtime(&info.MenorPosicion.fecha);
        //strftime(tiempoMenor, sizeof(tiempoMenor), "%a %Y-%m-%d %H:%M:%S", tm);
        sprintf(buffer + strlen(buffer), "%s %i %i %s",
                "Menor posicion",
                info.MenorPosicion.nEscritura,
                info.MenorPosicion.nRegistro,
                asctime(tm));
        tm = localtime(&info.MayorPosicion.fecha);
        //strftime(tiempoMayor, sizeof(tiempoMayor), "%a %Y-%m-%d %H:%M:%S", tm);

        sprintf(buffer + strlen(buffer), "%s %i %i %s\n\n",
                "Mayor posicion",
                info.MayorPosicion.nEscritura,
                info.MayorPosicion.nRegistro,
                asctime(tm));

        /*sprintf(buffer,
                "PID: %d\nNumero de escrituras:\t%d\n"
                "Primera escritura:\t%d\t%d\t%s\n"
                "Ultima escritura:\t%d\t%d\t%s\n"
                "Menor posición:\t\t%d\t%d\t%s\n"
                "Mayor posición:\t\t%d\t%d\t%s\n\n",
                info.pid, info.nEscrituras,
                info.PrimeraEscritura.nEscritura,
                info.PrimeraEscritura.nRegistro,
                tiempoPrimero,
                info.UltimaEscritura.nEscritura,
                info.UltimaEscritura.nRegistro,
                tiempoUltimo,
                info.MenorPosicion.nEscritura,
                info.MenorPosicion.nRegistro,
                tiempoMenor,
                info.MayorPosicion.nEscritura,
                info.MayorPosicion.nRegistro,
                tiempoMayor);*/
        if ((nbytes += mi_write(nfichero, &buffer, nbytes, strlen(buffer))) < 0)
        {
            perror("ERROR EN verificacion.c AL ESCRIBIR EL EL FICHERO");
            bumount();
            return -1;
        }
    }
    bumount();
}