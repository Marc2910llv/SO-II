/*
 *   Marc Llobera Villalonga
 */

#include "directorios.h"
#define DEBUG1 1

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

/// @brief
/// @param camino
/// @param buffer
/// @param tipo
/// @return
int mi_dir(const char *camino, char *buffer, char tipo)
{
}

/// @brief
/// @param camino
/// @param permisos
/// @return
int mi_chmod(const char *camino, unsigned char permisos)
{
}

/// @brief
/// @param camino
/// @param p_stat
/// @return
int mi_stat(const char *camino, struct STAT *p_stat)
{
}