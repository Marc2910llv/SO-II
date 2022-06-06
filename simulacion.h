#include "directorios.h"
#define REGMAX 500000
#define PROCESOS 3
#define ESCRITURAS 10
struct REGISTRO{
    time_t fecha;
    pid_t pid;
    int nEscritura;
    int nRegistro;
};

void my_sleep(unsigned msec);
void reaper();
