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

    struct STAT st;
    mi_stat(argv[2], &st);

    int numentradas = (st.tamEnBytesLog / sizeof(struct entrada));
    if (numentradas != PROCESOS)
    {
        perror("ERROR EN verificacion.c, EL NÚMERO DE ENTRADAS NO ES CORRECTO");
        // bunmount();
        return -1;
    }

    char nfichero[500];
    sprintf(nfichero, "%informe.txt", argv[2]);
    if (mi_creat(nfichero, 7) < 0)
    {
        perror("ERROR EN verificacion.c, EL NÚMERO DE ENTRADAS NO ES CORRECTO");
        // bunmount(argv[1]);
        exit(0);
    }

    struct entrada entradas[numentradas];
    int error = mi_read(argv[2], entradas, 0, sizeof(entradas));
    if (error < 0)
    {
        mostrar_error_buscar_entrada(error);
        return -1;
    }

    for (int nentrada = 0; nentrada < numentradas; nentrada++) //para cada entrada de directorio de un proceso
    {
        pid_t pid = atoi(strchr(entradas[nentrada].nombre, '_') + 1); // sacamos el pid a traves del nombre
        struct INFORMACION info;
        info.pid = pid;
        info.nEscrituras = 0;

        char camino_prueba[128];
        sprintf(camino_prueba, "%s%s/prueba.dat", argv[2], entradas[nentrada].nombre);

        struct REGISTRO buffer_rescrituras[1024 * 5];
        memset(buffer_rescrituras, 0, sizeof(buffer_rescrituras));

        int offset = 0;
        // Mientras haya escrituras en prueba.dat hacer
        while (mi_read(camino_prueba, buffer_rescrituras, offset, sizeof(buffer_rescrituras)) > 0)
        {
            int nregistro = 0;
            while (nregistro < 1024 * 5)
            {
                //si la escritura es válida entonces:
                if(buffer_rescrituras[nregistro].pid == info.pid)
                {
                    if(!info.nEscrituras)
                    {//si es la 1ra
                    //Inicializar los registros significativos con los datos de esa escritura 
                    info.MayorPosicion = buffer_rescrituras[nregistro];
                    info.MenorPosicion = buffer_rescrituras[nregistro];
                    info.PrimeraEscritura = buffer_rescrituras[nregistro];
                    info.UltimaEscritura = buffer_rescrituras[nregistro];
                    info.nEscrituras ++;
                    }
                    else
                    { //obtener 1ra y últ. num escritura y actualizar si cabe                       
                        if (buffer_escrituras[nregistro].nRegistro > info.MayorPosicion.nRegistro)
                        {
                            info.MayorPosicion = buffer_escrituras[nregistro];
                        }
                        if (buffer_escrituras[nregistro].nRegistro < info.MenorPosicion.nRegistro)
                        {
                            info.MenorPosicion = buffer_escrituras[nregistro];
                        }
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
                        info.nEscrituras++;
                    }                   
                }
                //incrementar contador escrituras válidas
                nregistro++;
            }
             //Obtener la escritura de la última posición
             //Añadir la información del struct info al fichero informe.txt por el final
             //no tenc ni idea
        }
    }
    //desmontar el dispositivo
    //tampoc se que es, falta ses mejoras, i lo des time ;
}