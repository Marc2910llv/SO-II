#include <sys/wait.h>
#include <signal.h>
#include "simulacion.h"
#include <time.h>

#define DEBUG2 0

int acabados =0;

void reaper(){
    pid_t ended;
    signal(SIGCHLD,reaper);
    while((ended = waitpid(-1,NULL,WNOHANG))>0){
        acabados++;
        #if DEBUG2
        fprintf(stderr, "[simulación.c → Acabado proceso con PID %d, total acabados: %d\n", ended, acabados);
        #endif
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
    //tendremos que crear un array con 21 espacios +1
    char temps[15]; // aaaammddhhmms = 14 caracteres +1
    memset(temps,0,sizeof(temps));
    time_t tim = time(NULL);
    struct tm *tm = localtime(&tim);
    int any = (1900+tm->tm_year);
    sprintf(temps,"%d%02d%02d%02d%02d%02d",any,tm->tm_mon+1,tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
    char cami[25];
    memset(cami,0,sizeof(cami));
    strcpy(cami,"/simul_");
    strcat(cami,temps);
    strcat(cami,"/");


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
            memset(directori,0,sizeof(directori));
            sprintf(directori,"%sproceso_%d/",cami,getpid());
            if(mi_creat(directori,7)<0){
                bumount();
                exit(0);
            }
            char ficher[113];
            memset(ficher,0,sizeof(ficher));
            sprintf(ficher,"%sprueba.dat",directori);
            if(mi_creat(ficher,7)<0){
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
                mi_write(ficher,&registre,registre.nRegistro * sizeof(struct REGISTRO),sizeof(struct REGISTRO));
                #if DEBUG2
                fprintf(stderr, "[simulación.c → Escritura %i en %s]\n", nescritura, ficher);
                #endif
                my_sleep(50);
            }
            fprintf(stderr, "[Proceso %d : Completadas %d escrituras en %s]\n",proceso,ESCRITURAS,ficher);
            bumount();
            exit(0);
        }
        my_sleep(150);
    }

    while(acabados<PROCESOS){
        pause();
    }
    fprintf(stderr, "Total de procesos terminados: %d\n", acabados);
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
