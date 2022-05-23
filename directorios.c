// Pere Joan Vives Morey
// Marc Llobera Villalonga
// Carlos Lozano Alemañy
#include "directorios.h"
#include <string.h>

// Dada una cadena de caracteres camino (que comience por '/'), separa su contenido en dos
int extraer_camino(const char *camino, char *inicial, char *final, char *tipo)
{
    char delimitador[2] = "/";
    char str[strlen(camino)];
    strcpy(str,camino);
    char *token;

    if(camino[0]!='/'){
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

    struct superbloque SB;
    memset(inicial, 0, sizeof(entrada.nombre));
    memset(final, 0, strlen(camino_parcial));
    memset(entrada.nombre, 0, sizeof(entrada.nombre));

    if (!strcmp(camino_parcial, "/"))
    {                               // camino_parcial es “/”
        if (bread(0, &SB) == -1)
        {
            perror("ERROR EN leer_sf AL LEER EL SUPERBLOQUE");
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
    printf("[buscar_entrada()->inicial: %s, final: %s, reservar: %d]\n", inicial,final, reservar);

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

    struct entrada buffer_lectura[BLOCKSIZE / sizeof(struct entrada)];
    memset(buffer_lectura, 0, (BLOCKSIZE / sizeof(struct entrada)) * sizeof(struct entrada));
    // el buffer de lectura puede ser un struct tipo entrada
    // o mejor un array de las entradas que caben en un bloque, para optimizar la lectura en RAM

    cant_entradas_inodo = inodo.tamEnBytesLog / sizeof(struct entrada); // cantidad de entradas que contiene el inodo
    num_entrada_inodo = 0;                                                  // nº de entrada inicial
    int b_leidos = 0;
    if (cant_entradas_inodo > 0)
    {
        b_leidos += mi_read_f(*p_inodo_dir, &buffer_lectura, b_leidos, BLOCKSIZE);

        while ((num_entrada_inodo < cant_entradas_inodo) && (strcmp(inicial, buffer_lectura[num_entrada_inodo].nombre) != 0))
        {

            num_entrada_inodo++;
            if ((num_entrada_inodo % (BLOCKSIZE / sizeof(struct entrada))) == 0)
            {
                b_leidos += mi_read_f(*p_inodo_dir, &buffer_lectura, b_leidos, BLOCKSIZE);
            }
        }
    } //copiat

    if (strcmp(buffer_lectura[num_entrada_inodo].nombre, inicial) != 0)//copiat->únic fallo
    { // la entrada no existe
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
                        entrada.ninodo = reservar_inodo(tipo, 6);
                        printf("[buscar_entrada()->reservado inodo: %d tipo %c con permisos %d para '%s']\n", entrada.ninodo, tipo, permisos, entrada.nombre);
                    }
                    else
                    { // cuelgan más diretorios o ficheros
                        return ERROR_NO_EXISTE_DIRECTORIO_INTERMEDIO;
                    }
                }
                else
                { // es un fichero
                    entrada.ninodo = reservar_inodo('f', 6);
                    printf("[buscar()->reservado inodo: %d tipo %c con permisos %d para '%s']\n", entrada.ninodo, tipo, permisos, entrada.nombre);
                }
                fprintf(stderr, "[buscar_entrada()->creada entrada: %s, %d] \n", inicial, entrada.ninodo);
                if (mi_write_f(*p_inodo_dir, &entrada, inodo.tamEnBytesLog, sizeof(struct entrada)) == -1)
                {
                    if (entrada.ninodo != -1)
                    { // entrada.inodo != -1
                        liberar_inodo(entrada.ninodo);
                        fprintf(stderr, "[buscar_entrada()-> liberado inodo %i, reservado a %s\n", num_entrada_inodo, inicial);
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
        *p_inodo = buffer_lectura[num_entrada_inodo].ninodo; // asignar a *p_inodo el numero de inodo del directorio o fichero creado o leido
        *p_entrada = entrada.ninodo;  // asignar a *p_entrada el número de su entrada dentro del último directorio que lo contiene
        return 0;
    }
    else
    {
        *p_inodo_dir = buffer_lectura[num_entrada_inodo].ninodo; // asignamos a *p_inodo_dir el puntero al inodo que se indica en la entrada encontrada;
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
int mi_dir(const char *camino, char *buffer)
{
    struct tm *tm;
    unsigned int p_inodo_dir = 0;
    unsigned int p_inodo = 0;
    unsigned int p_entrada = 0;
    int error;

    if ((error = buscar_entrada(camino, &p_inodo_dir, &p_inodo, &p_entrada, 0, 4)) < 0)
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

    struct entrada entradas[BLOCKSIZE / sizeof(struct entrada)];
    memset(&entradas, 0, sizeof(struct entrada));

    int offset = 0;
    offset += mi_read_f(p_inodo, entradas, offset, BLOCKSIZE);

    for (int i = 0; i < inodo.tamEnBytesLog / sizeof(struct entrada); i++)
    {
        if (leer_inodo(entradas[i % (BLOCKSIZE / sizeof(struct entrada))].ninodo, &inodo) == -1)
        {
            perror("ERROR EN mi_dir AL INTENTAR LEER ALGUNO DE LOS INODOS CORRESPONDIENTES");
            return -1;
        }

         if (inodo.tipo == 'd')
            {
                
                strcat(buffer, "d");
            }
            else
            {
                
                strcat(buffer, "f");
            }
            strcat(buffer, "\t");

            strcat(buffer, ((inodo.permisos & 4) == 4) ? "r" : "-");
            strcat(buffer, ((inodo.permisos & 2) == 2) ? "w" : "-");
            strcat(buffer, ((inodo.permisos & 1) == 1) ? "x" : "-");
            strcat(buffer, "\t");


            tm = localtime(&inodo.mtime);
            sprintf(tmp, "%d-%02d-%02d %02d:%02d:%02d", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
            strcat(buffer, tmp);
            strcat(buffer, "\t");


            sprintf(tamEnBytes, "%d", inodo.tamEnBytesLog);
            strcat(buffer, tamEnBytes);
            strcat(buffer, "\t");


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
    return inodo.tamEnBytesLog / sizeof(struct entrada);
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