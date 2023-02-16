/*
 *   Marc Llobera Villalonga
 *
 *   mi_mkfs.c {main()}
 */

#include <string.h>
#include "bloques.h"

int main(int argc, char **argv)
{
    bmount(argv[1]);

    unsigned char buf[BLOCKSIZE];
    memset(buf, 0, BLOCKSIZE);

    for (int i = 0; i < atoi(argv[2]); i++)
    {
        bwrite(i, buf);
    }

    bumount();
}