#include "directorios.h"
#define REGMAX 500000
#define PROCESOS 100
#define ESCRITURAS 50
struct REGISTRO{
    time_t fecha;
    pid_t pid;
    int nEscritura;
    int nRegistro;
};

void my_sleep(unsigned msec);
void reaper();
