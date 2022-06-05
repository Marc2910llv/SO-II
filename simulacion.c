#include <sys/wait.h>
#include <signal.h>
#include "simulacion.h"
#include <time.h>

#define PROCESOS 100
#define ESCRITURAS 50

int acabados =0;

void reaper(){
    pid_t ended;
    signal(SIGCHLD,reaper);
    while((ended = waitpid(-1,NULL,WNOHANG))>0){
        acabados++;
    }
}

int main(int argc, char const *argv[]){
    signal(SIGCHLD,reaper);
    if(argc!=2){
        perror("Error de sintaxis: ./simulacion <disco> \n");
        return -1;
    }

    if(bmount(argv[1])==-1){
        perror("Error en simulación.c al intentar abrir el fichero\n");
    }

    // /simul_aaaammddhhmms/ = 21 carateres
    //tendremos que crear un array con 21 espacios
    char cami[21];
    char temps[14]; // aaaammddhhmms = 14 caracteres
    time_t tim = time(NULL);
    struct tm *tm = localtime(&tim);;
    int any = (1900+tm->tm_year);
    sprintf(temps,"%d%02d%02d%02d%02d%02d",any,tm->tm_mon,tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
    strcpy(cami,"/simul_");
    strcat(cami,temps);
    strcat(cami,"/");

    printf("Directori: %s\n",cami);
    
    if(mi_creat(cami,6)==-1){
        return -1;
    }
    for(int proceso=1;proceso<=PROCESOS;proceso++){
        pid_t pid = fork();
        if(pid==0){
            if(bmount(argv[1])==-1){
                perror("Error al abrir el archivo en hijo\n");
            }
            char directori[100];
            sprintf(directori,"%sproceso_%d/",cami,getpid());
            if(mi_creat(directori,6)<0){
                bumount();
                exit(0);
            }

            char ficher[100];
            sprintf(ficher,"%sprueba.dat",directori);
            if(mi_creat(ficher,4)<0){
                bumount();
                exit(0);
            }

            //inicializar la semilla de datos aleatorios
            srand(time(NULL) + getpid());
            for(int nescritura=1;nescritura<=ESCRITURAS;nescritura++){
                struct REGISTRO registre;
                registre.fecha = time(NULL);
                registre.pid= getpid();
                registre.nEscritura = nescritura;
                registre.nRegistro = rand() % REGMAX;
                mi_write(ficher,&nescritura,registre.nRegistro * sizeof(struct REGISTRO),sizeof(struct REGISTRO));
                my_sleep(50);
            }
            bumount();
            exit(0);
        }
        my_sleep(150);
    }

    while(acabados<100){
        pause();
    }
    bumount();
    exit(0);
}

void my_sleep(unsigned msec) { //recibe tiempo en milisegundos
   struct timespec req, rem;
   int err;
   req.tv_sec = msec / 1000; //conversión a segundos
   req.tv_nsec = (msec % 1000) * 1000000; //conversión a nanosegundos
   while ((req.tv_sec != 0) || (req.tv_nsec != 0)) {
       if (nanosleep(&req, &rem) == 0) 
           break;
       err = errno;
       // Interrupted; continue
       if (err == EINTR) {
           req.tv_sec = rem.tv_sec;
           req.tv_nsec = rem.tv_nsec;
       }
   }
}