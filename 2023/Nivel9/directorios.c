/*
 *   Marc Llobera Villalonga
 */

#include "directorios.h"
#define DEBUG1 1 // buscar_entrada

/// @brief dada una cadena de caracteres camino, separa su contenido en dos
/// @param camino
/// @param inicial
/// @param final
/// @param tipo
/// @return EXITO o FALLO
int extraer_camino(const char *camino, char *inicial, char *final, char *tipo)
{
    char delimitador[2] = "/";
    char cadena[strlen(camino)];
    strcpy(cadena, camino);
    char *token;

    if (camino[0] != '/')
    {
        return FALLO;
    }

    char *a = strchr((camino + 1), '/');
    if (a)
    {
        token = strtok(cadena, delimitador);
        strcpy(inicial, token);
        strcpy(final, a);
        strcpy(tipo, "d");
    }
    else
    {
        strcpy(inicial, (camino + 1));
        strcpy(final, "");
        strcpy(tipo, "f");
    }

    return EXITO;
}

/// @brief buscar una determinada entrada entre las entradas del inodo correspondiente a su directorio padre
/// @param camino_parcial
/// @param p_inodo_dir
/// @param p_inodo
/// @param p_entrada
/// @param reservar
/// @param permisos
/// @return EXITO, FALLO o error_buscar_entrada
int buscar_entrada(const char *camino_parcial, unsigned int *p_inodo_dir,
                   unsigned int *p_inodo, unsigned int *p_entrada, char reservar, unsigned char permisos)
{
    struct entrada entrada;
    union _inodo inodo;
    char inicial[sizeof(entrada.nombre)];
    char final[strlen(camino_parcial)];
    char tipo;
    int cant_entradas_inodo, num_entrada_inodo;

    memset(inicial, 0, sizeof(entrada.nombre));
    memset(final, 0, strlen(camino_parcial));
    memset(entrada.nombre, 0, sizeof(entrada.nombre));

    if (!strcmp(camino_parcial, "/"))
    { // camino_parciales “/”
        struct superbloque SB;
        if (bread(0, &SB) == FALLO)
        {
            perror("Error buscar_entrada bread (superbloque)");
            return FALLO;
        }
        *(p_inodo) = SB.posInodoRaiz; // nuestra raiz siempre estará asociada al inodo 0
        *(p_entrada) = 0;
        return EXITO;
    }

    if (extraer_camino(camino_parcial, inicial, final, &tipo) == FALLO)
    {
        return ERROR_CAMINO_INCORRECTO;
    }

#if DEBUG1
    printf("[buscar_entrada()->inicial: %s, final: %s, reservar: %d]\n", inicial, final, reservar);
#endif

    // buscamos la entrada cuyo nombre se encuentra en inicial
    if (leer_inodo(*p_inodo_dir, &inodo) == FALLO)
    {
        perror("Error buscar_entrada leer_inodo");
        return FALLO;
    }

    if ((inodo.permisos & 4) != 4)
    {
        return ERROR_PERMISO_LECTURA;
    }

    memset(entrada.nombre, 0, sizeof(entrada.nombre));
    // el buffer de lectura puede ser un struct tipo entrada
    // o mejor un array de las entradas que caben en un bloque, para optimizar la lectura en RAM

    cant_entradas_inodo = inodo.tamEnBytesLog / sizeof(struct entrada); // cantidad de entradas que contiene el inodo
    num_entrada_inodo = 0;                                              // nº de entrada inicial
    int tam = sizeof(struct entrada);

    if (cant_entradas_inodo > 0)
    {
        // struct entrada entradas[BLOCKSIZE];
        int error = mi_read_f(*p_inodo_dir, &entrada, num_entrada_inodo * tam, tam);
        if (error == -2)
        {
            return ERROR_PERMISO_LECTURA;
        }
        else if (error == FALLO)
        {
            perror("Error buscar_entrada mi_read_f");
            return FALLO;
        }

        /*for (int i = 0; (i < cant_entradas_inodo) && (inicial != entrada.nombre); i++)
        {
            entrada = entradas[i];
        }*/

        while ((num_entrada_inodo < cant_entradas_inodo) && (strcmp(inicial, entrada.nombre) != 0))
        {
            num_entrada_inodo++;
            memset(entrada.nombre, 0, sizeof(entrada.nombre));
            error = mi_read_f(*p_inodo_dir, &entrada, num_entrada_inodo * tam, tam);
            if (error == -2)
            {
                return ERROR_PERMISO_LECTURA;
            }
            else if (error == FALLO)
            {
                perror("Error buscar_entrada mi_read_f");
                return FALLO;
            }
        }
    }

    if ((strcmp(entrada.nombre, inicial) != 0) && (num_entrada_inodo == cant_entradas_inodo))
    {
        // la entrada no existe
        switch (reservar)
        {
        case 0: // modoconsulta.Comonoexisteretornamoserror
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

                if (mi_write_f(*p_inodo_dir, &entrada, num_entrada_inodo * sizeof(struct entrada), sizeof(struct entrada)) == FALLO)
                {
                    if (entrada.ninodo != -1)
                    { // entrada.inodo != -1
                        liberar_inodo(entrada.ninodo);
#if DEBUG1
                        fprintf(stderr, "[buscar_entrada()-> liberado inodo %i, reservado a %s\n", num_entrada_inodo, inicial);
#endif
                    }
                    return FALLO; // -1
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
        *p_inodo = entrada.ninodo;
        *p_entrada = num_entrada_inodo;
        return EXITO; // 0
    }
    else
    {
        *p_inodo_dir = entrada.ninodo;
        return buscar_entrada(final, p_inodo_dir, p_inodo, p_entrada, reservar, permisos);
    }

    return EXITO; // 0
}

/// @brief mostrar errores de buscar entrada
/// @param error
void mostrar_error_buscar_entrada(int error)
{
    // fprintf(stderr,"Error:%d\n",error);
    switch (error)
    {
    case -2:
        fprintf(stderr, RED "Error: Camino incorrecto.\n" RESET);
        break;
    case -3:
        fprintf(stderr, RED "Error: Permiso denegado de lectura.\n" RESET);
        break;
    case -4:
        fprintf(stderr, RED "Error: No existe el archivo o el directorio.\n" RESET);
        break;
    case -5:
        fprintf(stderr, RED "Error: No existe algún directorio intermedio.\n" RESET);
        break;
    case -6:
        fprintf(stderr, RED "Error: Permiso denegado de escritura.\n" RESET);
        break;
    case -7:
        fprintf(stderr, RED "Error: El archivo ya existe.\n" RESET);
        break;
    case -8:
        fprintf(stderr, RED "Error: No es un directorio.\n" RESET);
        break;
    }
}

/// @brief crear un fichero/directorio y su entrada de directorio
/// @param camino
/// @param permisos
/// @return EXITO, FALLO o error_buscar_entrada
int mi_creat(const char *camino, unsigned char permisos)
{
    unsigned int p_inodo_dir = 0; // Leer superbloque y asignarle posInodoRaiz
    unsigned int p_inodo = 0;
    unsigned int p_entrada = 0;
    return buscar_entrada(camino, &p_inodo_dir, &p_inodo, &p_entrada, 1, permisos);
}

/// @brief poner el contenido de un directorio en un buffer de memoria
/// @param camino
/// @param buffer
/// @param tipo
/// @return nentradas o FALLO
int mi_dir(const char *camino, char *buffer, char tipo)
{
    unsigned int p_inodo_dir = 0;
    unsigned int p_inodo = 0;
    unsigned int p_entrada = 0;

    int error = buscar_entrada(camino, &p_inodo_dir, &p_inodo, &p_entrada, 0, 4);
    if (error < FALLO)
    {
        return error;
    }
    else if (error == FALLO)
    {
        perror("Error mi_dir buscar_entrada");
        return FALLO;
    }

    union _inodo inodo;
    if (leer_inodo(p_inodo, &inodo) == FALLO)
    {
        perror("Error mi_dir leer_inodo");
        return FALLO;
    }

    if ((inodo.permisos & 4) != 4)
    {
        fprintf(stderr, RED "Error, no se tienen permisos de lectura\n" RESET);
        return FALLO;
    }

    char tmp[100];
    char tam[10];
    struct tm *tm;

    int nentradas = 0;

    if (camino[strlen(camino) - 1] == '/')
    {
        if (leer_inodo(p_inodo, &inodo) == FALLO)
        {
            perror("Error mi_dir leer_inodo");
            return FALLO;
        }
        tipo = inodo.tipo;

        struct entrada entradas[BLOCKSIZE / sizeof(struct entrada)];
        memset(&entradas, 0, sizeof(struct entrada));

        nentradas = inodo.tamEnBytesLog / sizeof(struct entrada);

        int offset = 0;
        offset = offset + mi_read_f(p_inodo, entradas, offset, BLOCKSIZE);
        if (offset == FALLO)
        {
            perror("Error mi_dir mi_read_f");
            return FALLO;
        }

        for (int i = 0; i < nentradas; i++)
        {
            if (leer_inodo(entradas[i % (BLOCKSIZE / sizeof(struct entrada))].ninodo, &inodo) == FALLO)
            {
                perror("Error mi_dir leer_inodo (dentro de bucle)");
                return FALLO;
            }

            if (inodo.tipo == 'd')
            {
                strcat(buffer, COLOR_DIRECTORIO "d");
            }
            else
            {
                strcat(buffer, COLOR_FICHERO "f");
            }
            strcat(buffer, "\t");

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

            tm = localtime(&inodo.mtime);
            sprintf(tmp, "%d-%02d-%02d %02d:%02d:%02d", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
            strcat(buffer, tmp);
            strcat(buffer, "    ");

            sprintf(tam, "%d", inodo.tamEnBytesLog);
            strcat(buffer, tam);
            strcat(buffer, "    ");

            strcat(buffer, entradas[i % (BLOCKSIZE / sizeof(struct entrada))].nombre);

            while ((strlen(buffer) % TAMFILA) != 0)
            {
                strcat(buffer, " ");
            }
            strcat(buffer, RESET "\n");

            if (offset % (BLOCKSIZE / sizeof(struct entrada)) == 0)
            {
                offset += mi_read_f(p_inodo, entradas, offset, BLOCKSIZE);
                if (offset == FALLO)
                {
                    perror("Error mi_dir mi_read_f (dentro del bucle)");
                    return FALLO;
                }
            }
        }
    }
    else
    {
        struct entrada entrada;
        if (mi_read_f(p_inodo_dir, &entrada, sizeof(struct entrada) * p_entrada, sizeof(struct entrada)) == FALLO)
        {
            perror("Error mi_dir mi_read_f");
            return FALLO;
        }

        if (leer_inodo(entrada.ninodo, &inodo) == FALLO)
        {
            perror("Error mi_dir leer inodo");
            return FALLO;
        }
        tipo = inodo.tipo;

        if (inodo.tipo == 'd')
        {
            strcat(buffer, COLOR_DIRECTORIO "d");
        }
        else
        {
            strcat(buffer, COLOR_FICHERO "f");
        }
        strcat(buffer, "\t");

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

        tm = localtime(&inodo.mtime);
        sprintf(tmp, "%d-%02d-%02d %02d:%02d:%02d", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
        strcat(buffer, tmp);
        strcat(buffer, "    ");

        sprintf(tam, "%d", inodo.tamEnBytesLog);
        strcat(buffer, tam);
        strcat(buffer, "    ");

        strcat(buffer, entrada.nombre);

        while ((strlen(buffer) % TAMFILA) != 0)
        {
            strcat(buffer, " ");
        }
        strcat(buffer, RESET "\n");
    }

    return nentradas;
}

/// @brief buscar la entrada *camino con la función buscar_entrada, si existe llamamos a mi_chmod_f con el ninodo
/// @param camino
/// @param permisos
/// @return EXITO o FALLO
int mi_chmod(const char *camino, unsigned char permisos)
{
    unsigned int p_inodo_dir = 0;
    unsigned int p_inodo = 0;
    unsigned int p_entrada = 0;

    int error = buscar_entrada(camino, &p_inodo_dir, &p_inodo, &p_entrada, 0, permisos);
    if (error < 0)
    {
        return error;
    }

    if (mi_chmod_f(p_inodo, permisos) == FALLO)
    {
        perror("Error mi_chmod mi_chmod_f");
        return FALLO;
    }

    return EXITO;
}

/// @brief buscar la entrada *camino con la función buscar_entrada, si existe llamamos a mi_stat_f con el ninodo
/// @param camino
/// @param p_stat
/// @return EXITO o FALLO
int mi_stat(const char *camino, struct STAT *p_stat)
{
    unsigned int p_inodo_dir = 0;
    unsigned int p_inodo = 0;
    unsigned int p_entrada = 0;

    int error = buscar_entrada(camino, &p_inodo_dir, &p_inodo, &p_entrada, 0, p_stat->permisos);
    if (error < 0)
    {
        return error;
    }

    if (mi_stat_f(p_inodo, p_stat) == FALLO)
    {
        perror("Error mi_stat mi_stat_f");
        return FALLO;
    }

    return EXITO;
}

/// @brief
/// @param camino
/// @param buf
/// @param offset
/// @param nbytes
/// @return
int mi_write(const char *camino, const void *buf, unsigned int offset, unsigned int nbytes)
{
}

/// @brief
/// @param camino
/// @param buf
/// @param offset
/// @param nbytes
/// @return
int mi_read(const char *camino, void *buf, unsigned int offset, unsigned int nbytes)
{
}