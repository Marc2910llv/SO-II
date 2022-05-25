// Pere Joan Vives Morey
// Marc Llobera Villalonga
// Carlos Lozano Alemañy
#include "directorios.h"
#include <string.h>
#define DEBUG1 1

static struct UltimaEntrada UltimaEntrada[CACHE];

// Dada una cadena de caracteres camino (que comience por '/'), separa su contenido en dos
int extraer_camino(const char *camino, char *inicial, char *final, char *tipo)
{
    char delimitador[2] = "/";
    char str[strlen(camino)];
    strcpy(str,camino);
    char *token;

    if (camino[0] != '/')
    {
        return -1;
    }

    char *fin = strchr((camino+1),'/');
    if(fin){
        token = strtok(str,delimitador);
        strcpy(inicial,token);
        strcpy(final,fin);
        strcpy(tipo,"d");
    }else{
        strcpy(inicial, (camino + 1));
        strcpy(final,"");
        strcpy(tipo,"f");  
    }

    return 0;
    
}

// buscará una determinada entrada (la parte *inicial del *camino_parcial que nos devuelva extraer_camino()) entre las entradas del inodo correspondiente a su directorio padre
int buscar_entrada(const char *camino_parcial, unsigned int *p_inodo_dir, unsigned int *p_inodo, unsigned int *p_entrada, char reservar, unsigned char permisos)
{
    struct entrada entrada;
    struct inodo inodo;
    char inicial[sizeof(entrada.nombre)];
    char final[strlen(camino_parcial)];
    char tipo;
    int cant_entradas_inodo, num_entrada_inodo;

    memset(inicial, 0, sizeof(entrada.nombre));
    memset(final, 0, strlen(camino_parcial));
    memset(entrada.nombre, 0, sizeof(entrada.nombre));

    if (!strcmp(camino_parcial, "/"))
    { // camino_parcial es “/”
        struct superbloque SB;

        if (bread(0, &SB) == -1)
        {
            perror("ERROR EN buscar_entrada AL LEER EL SUPERBLOQUE");
            return -1;
        }
        *(p_inodo) = SB.posInodoRaiz; // nuestra raiz siempre estará asociada al inodo 0
        *(p_entrada) = 0;
        return 0;
    }

    if (extraer_camino(camino_parcial, inicial, final, &tipo) == -1)
    {
        perror("ERROR EN buscar_entrada AL INTENTAR EXTRAER EL CAMINO");
        return ERROR_CAMINO_INCORRECTO;
    }
#if DEBUG1
    printf("[buscar_entrada()->inicial: %s, final: %s, reservar: %d]\n", inicial, final, reservar);
#endif
    // buscamos la entrada cuyo nombre se encuentra en inicial
    if (leer_inodo(*p_inodo_dir, &inodo) == -1)
    {
        perror("ERROR EN buscar_entrada AL BUSCAR LA ENTRADA CUYO NOMBRE SE ENCUENTRA EN INICIAL");
        return -1;
    }
    if ((inodo.permisos & 4) != 4)
    {
        return ERROR_PERMISO_LECTURA;
    }

       memset(entrada.nombre, 0, sizeof(entrada.nombre));// el buffer de lectura puede ser un struct tipo entrada
    // o mejor un array de las entradas que caben en un bloque, para optimizar la lectura en RAM

    cant_entradas_inodo = inodo.tamEnBytesLog / sizeof(struct entrada); // cantidad de entradas que contiene el inodo
    num_entrada_inodo = 0;                                              // nº de entrada inicial

    if (cant_entradas_inodo > 0)
    {
      if (mi_read_f(*p_inodo_dir, &entrada, num_entrada_inodo * sizeof(struct entrada), sizeof(struct entrada)) < 0)
        {
            return ERROR_PERMISO_LECTURA;
        }
        while ((num_entrada_inodo < cant_entradas_inodo) && (strcmp(inicial, entrada.nombre) != 0))
        {

            num_entrada_inodo++;
            memset(entrada.nombre, 0, sizeof(entrada.nombre));
            if (mi_read_f(*p_inodo_dir, &entrada, num_entrada_inodo * sizeof(struct entrada), sizeof(struct entrada)) < 0)
            {
                return ERROR_PERMISO_LECTURA;
            }
        }
    } // copiat

    if ((num_entrada_inodo == cant_entradas_inodo) && (strcmp(entrada.nombre, inicial) != 0)) // copiat->únic fallo
    {                                                                   // la entrada no existe
        switch (reservar)
        {
        case 0: // modo consulta. Como no existe retornamos error
            return ERROR_NO_EXISTE_ENTRADA_CONSULTA;
            break;
        case 1: // modo escritura
                // Creamos la entrada en el directorio referenciado por *p_inodo_dir
            // si es fichero no permitir escritura
            if (inodo.tipo == 'f')
            {
                return ERROR_NO_SE_PUEDE_CREAR_ENTRADA_EN_UN_FICHERO;
            }
            // si es directorio comprobar que tiene permiso de escritura
            if ((inodo.permisos & 2) != 2)
            {
                return ERROR_PERMISO_ESCRITURA;
            }
            else
            {
                strcpy(entrada.nombre, inicial);
                if (tipo == 'd')
                {
                    if (strcmp(final, "/") == 0)
                    {
                        entrada.ninodo = reservar_inodo('d', permisos);
                        #if DEBUG1
                        printf("[buscar_entrada()->reservado inodo: %d tipo %c con permisos %d para '%s']\n", entrada.ninodo, tipo, permisos, entrada.nombre);
                        #endif
                    }
                    else
                    { // cuelgan más diretorios o ficheros
                        return ERROR_NO_EXISTE_DIRECTORIO_INTERMEDIO;
                    }
                }
                else
                { // es un fichero
                    entrada.ninodo = reservar_inodo('f', permisos);
                    #if DEBUG1
                    printf("[buscar()->reservado inodo: %d tipo %c con permisos %d para '%s']\n", entrada.ninodo, tipo, permisos, entrada.nombre);
                    #endif
                }
                #if DEBUG1
                fprintf(stderr, "[buscar_entrada()->creada entrada: %s, %d] \n", inicial, entrada.ninodo);
                #endif
                if (mi_write_f(*p_inodo_dir, &entrada, num_entrada_inodo * sizeof(struct entrada), sizeof(struct entrada)) == -1)
                {
                    if (entrada.ninodo != -1)
                    { // entrada.inodo != -1
                        liberar_inodo(entrada.ninodo);
                        #if DEBUG1
                        fprintf(stderr, "[buscar_entrada()-> liberado inodo %i, reservado a %s\n", num_entrada_inodo, inicial);
                        #endif
                    }
                    return -1; //-1
                }
            }
        }
    }
    if (!strcmp(final, "/") || !strcmp(final, ""))
    {
        if ((num_entrada_inodo < cant_entradas_inodo) && (reservar == 1))
        {
            // modo escritura y la entrada ya existe
            return ERROR_ENTRADA_YA_EXISTENTE;
        }
        // cortamos la recursividad
        *p_inodo = entrada.ninodo; // asignar a *p_inodo el numero de inodo del directorio o fichero creado o leido
        *p_entrada = num_entrada_inodo;                         // asignar a *p_entrada el número de su entrada dentro del último directorio que lo contiene
        return 0;
    }
    else
    {
        *p_inodo_dir = entrada.ninodo; // asignamos a *p_inodo_dir el puntero al inodo que se indica en la entrada encontrada;
        return buscar_entrada(final, p_inodo_dir, p_inodo, p_entrada, reservar, permisos);
    }
    return 0;
}

void mostrar_error_buscar_entrada(int error)
{
    switch (error)
    {
    case -1:
        fprintf(stderr, "Error: Camino incorrecto.\n");
        break;
    case -2:
        fprintf(stderr, "Error: Permiso denegado de lectura.\n");
        break;
    case -3:
        fprintf(stderr, "Error: No existe el archivo o el directorio.\n");
        break;
    case -4:
        fprintf(stderr, "Error: No existe algún directorio intermedio.\n");
        break;
    case -5:
        fprintf(stderr, "Error: Permiso denegado de escritura.\n");
        break;
    case -6:
        fprintf(stderr, "Error: El archivo ya existe.\n");
        break;
    case -7:
        fprintf(stderr, "Error: No es un directorio.\n");
        break;
    }
}
// Función de la capa de directorios que crea un fichero/directorio y su entrada de directorio
int mi_creat(const char *camino, unsigned char permisos)
{
    unsigned int p_inodo_dir = 0;
    unsigned int p_inodo = 0;
    unsigned int p_entrada = 0;
    int error;

    if ((error = buscar_entrada(camino, &p_inodo_dir, &p_inodo, &p_entrada, 1, permisos)) < 0)
    {
        return error;
    }
    return 0;
}

// Función de la capa de directorios que pone el contenido del directorio en un buffer de memoria y devuelve el número de entradas
int mi_dir(const char *camino, char *buffer, char tipo)
{
    struct tm *tm;
    unsigned int p_inodo_dir = 0;
    unsigned int p_inodo = 0;
    unsigned int p_entrada = 0;
    int nEntradas = 0;

    int error = buscar_entrada(camino, &p_inodo_dir, &p_inodo, &p_entrada, 0, 4);
    if (error < 0)
    {
        mostrar_error_buscar_entrada(error);
        return error;
    }

    struct inodo inodo;
    if (leer_inodo(p_inodo, &inodo) == -1)
    {
        perror("ERROR EN mi_dir AL INTENTAR LEER EL INODO");
        return -1;
    }
    if ((inodo.permisos & 4) != 4)
    {
        perror("ERROR EN mi_dir AL AL COMPROBAR LOS PERMISOS DEL INODO");
        return -1;
    }

    char tmp[100];
    char tamEnBytes[10];
    struct entrada entrada;

    if (camino[(strlen(camino)) - 1] == '/')
    {
        if (leer_inodo(p_inodo, &inodo) == -1)
        {
            return -1;
        }
        tipo = inodo.tipo;

        struct entrada entradas[BLOCKSIZE / sizeof(struct entrada)];
        memset(&entradas, 0, sizeof(struct entrada));

        nEntradas = inodo.tamEnBytesLog / sizeof(struct entrada);

        int offset = 0;
        offset += mi_read_f(p_inodo, entradas, offset, BLOCKSIZE);

        for (int i = 0; i < nEntradas; i++)
        {
            if (leer_inodo(entradas[i % (BLOCKSIZE / sizeof(struct entrada))].ninodo, &inodo) == -1)
            {
                return -1;
            }

            // Tipo
            if (inodo.tipo == 'd')
            {
                strcat(buffer, "d");
            }
            else
            {
                strcat(buffer, "f");
            }
            strcat(buffer, "\t");

            // Permisos
            if (inodo.permisos & 4)
            {
                strcat(buffer, "r");
            }
            else
            {
                strcat(buffer, "-");
            }
            if (inodo.permisos & 2)
            {
                strcat(buffer, "w");
            }
            else
            {
                strcat(buffer, "-");
            }
            if (inodo.permisos & 1)
            {
                strcat(buffer, "x");
            }
            else
            {
                strcat(buffer, "-");
            }
            strcat(buffer, "    ");

            // mTime
            tm = localtime(&inodo.mtime);
            sprintf(tmp, "%d-%02d-%02d %02d:%02d:%02d", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
            strcat(buffer, tmp);
            strcat(buffer, "    ");

            // Tamaño
            sprintf(tamEnBytes, "%d", inodo.tamEnBytesLog);
            strcat(buffer, tamEnBytes);
            strcat(buffer, "    ");

            // Nombre
            strcat(buffer, entradas[i % (BLOCKSIZE / sizeof(struct entrada))].nombre);
            while ((strlen(buffer) % TAMFILA) != 0)
            {
                strcat(buffer, " ");
            }
            strcat(buffer, "\n");

            if (offset % (BLOCKSIZE / sizeof(struct entrada)) == 0)
            {
                offset += mi_read_f(p_inodo, entradas, offset, BLOCKSIZE);
            }
        }
    }
    else
    { // Es un fichero
        mi_read_f(p_inodo_dir, &entrada, sizeof(struct entrada) * p_entrada, sizeof(struct entrada));
        leer_inodo(entrada.ninodo, &inodo);
        tipo = inodo.tipo;

        // Tipo
        if (inodo.tipo == 'd')
        {
            strcat(buffer, "d");
        }
        else
        {
            strcat(buffer, "f");
        }
        strcat(buffer, "    ");

        // Permisos
        if (inodo.permisos & 4)
        {
            strcat(buffer, "r");
        }
        else
        {
            strcat(buffer, "-");
        }
        if (inodo.permisos & 2)
        {
            strcat(buffer, "w");
        }
        else
        {
            strcat(buffer, "-");
        }
        if (inodo.permisos & 1)
        {
            strcat(buffer, "x");
        }
        else
        {
            strcat(buffer, "-");
        }
        strcat(buffer, "    ");

        // mTime
        tm = localtime(&inodo.mtime);
        sprintf(tmp, "%d-%02d-%02d %02d:%02d:%02d", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
        strcat(buffer, tmp);
        strcat(buffer, "    ");

        // Tamaño
        sprintf(tamEnBytes, "%d", inodo.tamEnBytesLog);
        strcat(buffer, tamEnBytes);
        strcat(buffer, "    ");

        // Nombre
        strcat(buffer, entrada.nombre);
        while ((strlen(buffer) % TAMFILA) != 0)
        {
            strcat(buffer, " ");
        }
        strcat(buffer, "\n");
    }
    return nEntradas;
}

// Buscar la entrada *camino con buscar_entrada() para obtener el nº de inodo (p_inodo).  Si la entrada existe llamamos a la función correspondiente de ficheros.c pasándole el p_inodo
int mi_chmod(const char *camino, unsigned char permisos)
{
    unsigned int p_inodo_dir = 0;
    unsigned int p_inodo = 0;
    unsigned int p_entrada = 0;
    int error;

    if ((error = buscar_entrada(camino, &p_inodo_dir, &p_inodo, &p_entrada, 0, permisos)) < 0)
    {
        return error;
    }

    if (mi_chmod_f(p_inodo, permisos) == -1)
    {
        perror("ERROR EN mi_chmod");
        return -1;
    }

    return 0;
}

// Buscar la entrada *camino con buscar_entrada() para obtener el p_inodo. Si la entrada existe llamamos a la función correspondiente de ficheros.c pasándole el p_inodo
int mi_stat(const char *camino, struct STAT *p_stat)
{
    unsigned int p_inodo_dir = 0;
    unsigned int p_inodo = 0;
    unsigned int p_entrada = 0;
    int error;

    if ((error = buscar_entrada(camino, &p_inodo_dir, &p_inodo, &p_entrada, 0, p_stat->permisos)) < 0)
    {
        perror("ERROR EN mi_stat AL INTENTAR OBTENER EL P_INODO");
        return error;
    }

    if (mi_stat_f(p_inodo, p_stat) == -1)
    {
        perror("ERROR EN mi_stat");
        return -1;
    }

    return p_inodo;
}

int MAXCACHE = CACHE;

// Escribir contenido en un fichero
int mi_write(const char *camino, const void *buf, unsigned int offset, unsigned int nbytes)
{
    unsigned int p_inodo_dir = 0;
    unsigned int p_inodo = 0;
    unsigned int p_entrada = 0;
    unsigned int escrituraMismoInodo = 0; // false

    // Recorrido del cache
    for (int i = 0; i < (MAXCACHE - 1); i++)
    {
        if (strcmp(camino, UltimaEntrada[i].camino) == 0)
        {
            p_inodo = UltimaEntrada[i].p_inodo;
            escrituraMismoInodo = 1; // true
            break;
        }
    }
    if (!escrituraMismoInodo) // if not true
    {
        int error = buscar_entrada(camino, &p_inodo_dir, &p_inodo, &p_entrada, 0, 4);
        if (error < 0)
        {
            return error;
        }

        if (MAXCACHE > 0)
        {
            strcpy(UltimaEntrada[CACHE - MAXCACHE].camino, camino);
            UltimaEntrada[CACHE - MAXCACHE].p_inodo = p_inodo;
            MAXCACHE--;
        }
        else // Remplazo FIFO
        {
            for (int i = 0; i < CACHE - 1; i++)
            {
                strcpy(UltimaEntrada[i].camino, UltimaEntrada[i + 1].camino);
                UltimaEntrada[i].p_inodo = UltimaEntrada[i + 1].p_inodo;
            }
            strcpy(UltimaEntrada[CACHE - 1].camino, camino);
            UltimaEntrada[CACHE - 1].p_inodo = p_inodo;
        }
    }

    int bytes_escritos = mi_write_f(p_inodo, buf, offset, nbytes);
    if (bytes_escritos == -1)
    {
        bytes_escritos = 0;
    }
    return bytes_escritos;
}

// Leer los nbytes del fichero indicado por camino, a partir del offset pasado por parámetro y copiarlos en el buffer buf
int mi_read(const char *camino, void *buf, unsigned int offset, unsigned int nbytes)
{
    unsigned int p_inodo_dir = 0;
    unsigned int p_inodo = 0;
    unsigned int p_entrada = 0;
    unsigned int escrituraMismoInodo = 0; // false

    // Recorrido del cache
    for (int i = 0; i < (MAXCACHE - 1); i++)
    {
        if (strcmp(camino, UltimaEntrada[i].camino) == 0)
        {
            p_inodo = UltimaEntrada[i].p_inodo;
            escrituraMismoInodo = 1; // true
            break;
        }
    }

    if (!escrituraMismoInodo) // if not true
    {
        int error = buscar_entrada(camino, &p_inodo_dir, &p_inodo, &p_entrada, 0, 4);
        if (error < 0)
        {
            return error;
        }

        if (MAXCACHE > 0)
        {
            strcpy(UltimaEntrada[CACHE - MAXCACHE].camino, camino);
            UltimaEntrada[CACHE - MAXCACHE].p_inodo = p_inodo;
            --MAXCACHE;
        }
        else // Remplazo FIFO
        {
            for (int i = 0; i < CACHE - 1; i++)
            {
                strcpy(UltimaEntrada[i].camino, UltimaEntrada[i + 1].camino);
                UltimaEntrada[i].p_inodo = UltimaEntrada[i + 1].p_inodo;
            }
            strcpy(UltimaEntrada[CACHE - 1].camino, camino);
            UltimaEntrada[CACHE - 1].p_inodo = p_inodo;
        }
    }

    int bytes_leidos = mi_read_f(p_inodo, buf, offset, nbytes);
    if (bytes_leidos == -1)
    {
        return ERROR_PERMISO_LECTURA;
    }
    return bytes_leidos;
}
// Crea el enlace de una entrada de directorio camino2 al inodo especificado por otra entrada de directorio camino1
int mi_link(const char *camino1, const char *camino2)
{
    unsigned int p_inodo_dir1 = 0;
    unsigned int p_inodo1 = 0;
    unsigned int p_entrada1 = 0;
    unsigned int p_inodo_dir2 = 0;
    unsigned int p_inodo2 = 0;
    unsigned int p_entrada2 = 0;

    struct inodo inodo;

    int error = buscar_entrada(camino1, &p_inodo_dir1, &p_inodo1, &p_entrada1, 0, 4);
    if (error < 0)
    {
        mostrar_error_buscar_entrada(error);
        return -1;
    }
    leer_inodo(p_inodo1, &inodo);
    if (inodo.tipo != 'f')
    {
        perror("ERROR EN mi_link, NO SE TRATA DE UN FICHERO");
        return -1;
    }
    if ((inodo.permisos & 4) != 4)
    {
        perror("ERROR CON LOS PERMISOS DE LECTURA EN mi_link");
        return ERROR_PERMISO_LECTURA;
    }

    error = buscar_entrada(camino2, &p_inodo_dir2, &p_inodo2, &p_entrada2, 1, 6);
    if (error < 0)
    {
        mostrar_error_buscar_entrada(error);
        return -1;
    }

    struct entrada entrada2;
    if (mi_read_f(p_inodo_dir2, &entrada2, sizeof(struct entrada) * (p_entrada2), sizeof(struct entrada)) < 0)
    {
        return -1;
    }

    entrada2.ninodo = p_inodo1;

    if (mi_write_f(p_inodo_dir2, &entrada2, sizeof(struct entrada) * (p_entrada2), sizeof(struct entrada)) < 0)
    {
        return -1;
    }

    if (liberar_inodo(p_inodo2) < 0)
    {
        return -1;
    }

    inodo.nlinks++;
    inodo.ctime = time(NULL);
    if (escribir_inodo(p_inodo1, inodo) == -1)
    {
        return -1;
    }
    return 0;
}

int mi_unlink(const char *camino)
{
    unsigned int p_inodo_dir = 0;
    unsigned int p_inodo = 0;
    unsigned int p_entrada = 0;

    int error = buscar_entrada(camino, &p_inodo_dir, &p_inodo, &p_entrada, 0, 4);
    if (error < 0)
    {
        mostrar_error_buscar_entrada(error);
        return -1;
    }

    struct inodo inodo;
    if (leer_inodo(p_inodo, &inodo) == -1)
    {
        return -1;
    }

    if ((inodo.tipo == 'd') && (inodo.tamEnBytesLog > 0))
    {
        return -1;
    }

    struct inodo inodo_dir;
    if (leer_inodo(p_inodo_dir, &inodo_dir) == -1)
    {
        return -1;
    }

    int num_entrada = inodo_dir.tamEnBytesLog / sizeof(struct entrada);

    if (p_entrada != num_entrada - 1)
    {
        struct entrada entrada;
        if (mi_read_f(p_inodo_dir, &entrada, sizeof(struct entrada) * (num_entrada - 1), sizeof(struct entrada)) < 0)
        {
            return -1;
        }

        if (mi_write_f(p_inodo_dir, &entrada, sizeof(struct entrada) * (p_entrada), sizeof(struct entrada)) < 0)
        {
            return -1;
        }
    }

    if (mi_truncar_f(p_inodo_dir, sizeof(struct entrada) * (num_entrada - 1)) == -1)
    {
        return -1;
    }

    inodo.nlinks--;

    if (!inodo.nlinks)
    {
        if (liberar_inodo(p_inodo) == -1)
        {
            return -1;
        }
    }
    else
    {
        inodo.ctime = time(NULL);
        if (escribir_inodo(p_inodo, inodo) == -1)
        {
            return -1;
        }
    }

    return 0;
}