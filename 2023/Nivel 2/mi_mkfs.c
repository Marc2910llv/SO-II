/*
 *   Marc Llobera Villalonga
 *
 *   mi_mkfs.c {main()}
 */

#include <string.h>
#include "ficheros_basico.h"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        perror("Error Sintaxis");
        return FALLO;
    }

    if (bmount(argv[1]) == FALLO)
    {
        perror("Error main bmount");
        return FALLO;
    }

    unsigned char buf[BLOCKSIZE];
    memset(buf, 0, BLOCKSIZE);

    for (int i = 0; i < atoi(argv[2]); i++)
    {
        if (bwrite(i, buf) == FALLO)
        {
            perror("Error main bwrite");
            return FALLO;
        }
    }

    if (initSB() == FALLO)
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

    if (bumount() == FALLO)
    {
        perror("Error main bumount");
        return FALLO;
    }
}