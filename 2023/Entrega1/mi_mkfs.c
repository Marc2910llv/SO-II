/*
 *   Marc Llobera Villalonga
 */

#include "ficheros_basico.h"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Sintaxisc corrrecta: mi_mkfs <nombre_dispositivo> <tamaño>\n");
        return FALLO;
    }

    unsigned char buf[BLOCKSIZE];
    memset(buf, 0, BLOCKSIZE);

    if (bmount(argv[1]) == FALLO)
    {
        perror("Error main bmount");
        return FALLO;
    }

    for (int i = 0; i < atoi(argv[2]); i++)
    {
        if (bwrite(i, buf) == FALLO)
        {
            perror("Error main bwrite");
            return FALLO;
        }
    }

    if (initSB(atoi(argv[2]), (atoi(argv[2]) / 4)) == FALLO)
    {
        perror("Error main initSB");
        return FALLO;
    }
    if (initMB() == FALLO)
    {
        perror("Error main initMB");
        return FALLO;
    }
    if (initAI() == FALLO)
    {
        perror("Error main initAI");
        return FALLO;
    }

    if (reservar_inodo('d', 7) == FALLO)
    {
        perror("Error main reservar_inodo");
        return FALLO;
    }

    if (bumount() == FALLO)
    {
        perror("Error main bumount");
    }
}