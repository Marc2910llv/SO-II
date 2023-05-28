/*
 *   Marc Llobera Villalonga
 */

#include "simulacion.h"
#include <time.h>

#define DEBUG1 0 // Escrituras

int acabados = 0;

int main(int argc, char const *argv[])
{
    signal(SIGCHLD, reaper);

    if (argc != 2)
    {
        fprintf(stderr, RED "Sintaxis: ./simulacion <disco>\n" RESET);
        return FALLO;
    }

    if (bmount(argv[1]) == FALLO) // padre
    {
        perror("Error main bmount (padre)");
        return FALLO;
    }

    // /simul_aaaammddhhmms/ = 21 carateres
    // tendremos que crear un array con 21 espacios +1
    char t[15]; // aaaammddhhmms = 14 caracteres +1
    memset(t, 0, sizeof(t));
    time_t tim = time(NULL);
    struct tm *tm = localtime(&tim);
    int año = (1900 + tm->tm_year);
    sprintf(t, "%d%02d%02d%02d%02d%02d", año, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
    char camino[25];
    memset(camino, 0, sizeof(camino));
    strcpy(camino, "/simul_");
    strcat(camino, t);
    strcat(camino, "/");

    int error = mi_creat(camino, 6);
    if (error == FALLO)
    {
        perror("Error main mi_creat");
    }
    else if (error < FALLO)
    {
        mostrar_error_buscar_entrada(error);
    }

    for (int proceso = 1; proceso <= NUMPROCESOS; proceso++)
    {
        pid_t pid = fork();
        if (pid == 0)
        {                                 // pid = 0
            if (bmount(argv[1]) == FALLO) // hijo
            {
                perror("Error main bmount (hijo)");
                return FALLO;
            }

            char dir[100];
            memset(dir, 0, sizeof(dir));
            sprintf(dir, "%sproceso_%d/", camino, getpid());
            error = mi_creat(dir, 7);
            if (error < EXITO)
            {
                if (error == FALLO)
                {
                    perror("Error main mi_creat (dir)");
                }
                else
                {
                    mostrar_error_buscar_entrada(error);
                }

                if (bumount() == FALLO) // hijo
                {
                    perror("Error main bumount (dir)");
                    return FALLO;
                }
                exit(0);
            }

            char fic[113];
            memset(fic, 0, sizeof(fic));
            sprintf(fic, "%sprueba.dat", dir);
            error = mi_creat(fic, 7);
            if (error < EXITO)
            {
                if (error == FALLO)
                {
                    perror("Error main mi_creat (fic)");
                }
                else
                {
                    mostrar_error_buscar_entrada(error);
                }

                if (bumount() == FALLO) // hijo
                {
                    perror("Error main bumount (fic)");
                    return FALLO;
                }
                exit(0);
            }

            srand(time(NULL) + getpid());

            for (int nescritura = 1; nescritura <= NUMESCRITURAS; nescritura++)
            { // 50
                struct REGISTRO registro;
                registro.fecha = time(NULL);
                registro.pid = getpid();
                registro.nEscritura = nescritura;
                registro.nRegistro = rand() % REGMAX; //[0, 499.999]

                int offset = registro.nRegistro * sizeof(struct REGISTRO);
                mi_write(fic, &registro, offset, sizeof(struct REGISTRO));
#if DEBUG1
                fprintf(stderr, "[simulación.c → Escritura %i en %s]\n", nescritura, fic);
#endif

                my_sleep(50);
            }
            fprintf(stderr, "[Proceso %d: Completadas %d escrituras en %s]\n", proceso, NUMESCRITURAS, fic);
            if (bumount() == FALLO) // hijo
            {
                perror("Error main bumount (hijo)");
                return FALLO;
            }
            exit(0); // Necesario para que se emita la señal SIGCHLD
        }
        my_sleep(150);
    }

    // Permitir que el padre espere por todos los hijos :
    while (acabados < NUMPROCESOS)
    {
        pause();
    }

    // fprintf(stderr, "Total de procesos terminados: %d\n", acabados);

    if (bumount() == FALLO) // padre
    {
        perror("Error main bumount (padre)");
        return FALLO;
    }

    exit(0); // o return 0
}

void reaper()
{
    pid_t ended;
    signal(SIGCHLD, reaper);
    while ((ended = waitpid(-1, NULL, WNOHANG)) > 0)
    {
        acabados++;
    }
}

void my_sleep(unsigned msec)
{ // recibe tiempo en milisegundos
    struct timespec req, rem;
    int err;
    req.tv_sec = msec / 1000;              // conversión a segundos
    req.tv_nsec = (msec % 1000) * 1000000; // conversión a nanosegundos
    while ((req.tv_sec != 0) || (req.tv_nsec != 0))
    {
        if (nanosleep(&req, &rem) == 0)
            // rem almacena el tiempo restante si una llamada al sistema
            // ha sido interrumpida por una señal
            break;
        err = errno;
        // Interrupted; continue
        if (err == EINTR)
        {
            req.tv_sec = rem.tv_sec;
            req.tv_nsec = rem.tv_nsec;
        }
    }
}