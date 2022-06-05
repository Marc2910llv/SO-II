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
    sprintf(nfichero, "%sinforme.txt", argv[2]);
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

    for (int nentrada = 0; nentrada < numentradas; nentrada++)
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
        // Mientras haya escrituras en prueba.dat
        while (mi_read(camino_prueba, buffer_rescrituras, offset, sizeof(buffer_rescrituras)) > 0)
        {
            int nregistro = 0;
            while (nregistro < 1024 * 5)
            {
            }
        }
    }
}