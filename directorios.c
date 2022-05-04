// Pere Joan Vives Morey
// Marc Llobera Villalonga
// Carlos Lozano Alemañy
#include "directorios.h"
#include <string.h>

int extraer_camino(const char *camino, char *inicial, char *final, char *tipo)
{
    char delimitador[] = "/";
    char *token;

    token = strtok(camino, delimitador);
    inicial = token;
    token = strtok(NULL, delimitador);
    if (token != NULL)
    {
        tipo = "d";
        final = "";
        char *buffer;
        while (token != NULL)
        {
            strcat(buffer, "/");
            strcat(buffer, token);
            token = strtok(NULL, delimitador);
        }
        final = buffer;
        return 1;
    }
    else
    {
        tipo = "f";
        final = "";
        return 0;
    }
    perror("ERROR EN extraer_camino AL SEPARAR LA RUTA DEL DIRECTORIO O EL FICHERO");
    return -1;
}

int buscar_entrada(const char *camino_parcial, unsigned int *p_inodo_dir, unsigned int *p_inodo, unsigned int *p_entrada, char reservar, unsigned char permisos)
{
    struct entrada entrada;
    struct inodo inodo *;
    char inicial[sizeof(entrada.nombre)];
    char final[strlen(camino_parcial)];
    char tipo;
    int cant_entradas_inodo, num_entrada_inodo;

    if (!strcmp(camino_parcial, "/"))
    {                               // camino_parcial es “/”
        *p_inodo = SB.posInodoRaiz; // nuestra raiz siempre estará asociada al inodo 0
        *p_entrada = 0;
        return 0;
    }

    if (extraer_camino(camino_parcial, inicial, final, &tipo) == -1)
    {
        perror("ERROR EN buscar_entrada AL INTENTAR EXTRAER EL CAMINO");
        return -1;
    }

    // buscamos la entrada cuyo nombre se encuentra en inicial
    if (leer_inodo(*p_inodo_dir, &inodo_dir) == -1)
    {
        perror("ERROR EN buscar_entrada AL BUSCAR LA ENTRADA CUYO NOMBRE SE ENCUENTRA EN INICIAL");
        return -1;
    }

    struct entrada buffer_lectura[BLOCKSIZE / sizeof(struct entrada)];
    memset(buffer_lectura, 0, (BLOCKSIZE / sizeof(struct entrada)) * sizeof(struct entrada));
    // el buffer de lectura puede ser un struct tipo entrada
    // o mejor un array de las entradas que caben en un bloque, para optimizar la lectura en RAM

    cant_entradas_inodo = dir_inodo.tamEnBytesLog / sizeof(struct entrada); // cantidad de entradas que contiene el inodo
    num_entrada_inodo = 0;                                                  // nº de entrada inicial
    if (cant_entradas_inodo > 0)
    {
        if (mi_read_f(*p_inodo_dir, &entrada, num_entrada_inodo * sizeof(struct entrada), sizeof(struct entrada)) < 0)
        {
            perror("ERROR EN buscar_entrada AL INTENTAR LEER LA ENTRADA");
            return -1;
        }
        while ((num_entrada_inodo < cant_entradas_inodo) && (inicial != entrada.nombre))
        {
            num_entrada_inodo++;
            // previamente volver a inicializar el buffer de lectura con 0s
            if (mi_read_f(*p_inodo_dir, &entrada, num_entrada_inodo * sizeof(struct entrada), sizeof(struct entrada)) < 0)
            {
                perror("ERROR EN buscar_entrada AL INTENTAR LEER LA ENTRADA");
                return -1;
            }
        }
    }

    if (inicial != entrada.nombre)
    { // la entrada no existe
        switch (reservar)
        {
        case 0: // modo consulta. Como no existe retornamos error
            return ERROR_NO_EXISTE_ENTRADA_CONSULTA;
            break;
        case 1: // modo escritura
                // Creamos la entrada en el directorio referenciado por *p_inodo_dir
            // si es fichero no permitir escritura
            if (inodo_dir.tipo == 'f')//REVISAR
            {
                return ERROR_NO_SE_PUEDE_CREAR_ENTRADA_EN_UN_FICHERO;
            }
            // si es directorio comprobar que tiene permiso de escritura
            if ((inodo_dir.permisos & 2) != 2)//REVISAR
            {
                return ERROR_PERMISO_ESCRITURA;
            }
            else
            {
                strcpy(entrada.nombre, inicial);
                if (tipo == 'd')
                {
                    if (fstrcmp(final, "/") == 0)
                    {
                        entrada.ninodo = reservar_inodo(tipo, permisos);
                    }
                    else
                    { // cuelgan más diretorios o ficheros
                        return ERROR_NO_EXISTE_DIRECTORIO_INTERMEDIO;
                    }
                }
                else
                { // es un fichero
                    entrada.ninodo = reservar_inodo(tipo, permisos);
                }
                if (mi_write_f(*p_inodo_dir, &entrada, dir_inodo.tamEnBytesLog, sizeof(struct entrada)) == -1)
                {
                    if (entrada.ninodo != -1)
                    { // entrada.inodo != -1
                        liberar_inodo(entrada.ninodo);
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
        *(p_inodo) = num_entrada_inodo; // asignar a *p_inodo el numero de inodo del directorio o fichero creado o leido
        *(p_entrada) = entrada.ninodo;  // asignar a *p_entrada el número de su entrada dentro del último directorio que lo contiene
        return 0;
    }
    else
    {
        *(p_inodo_dir) = entrada.ninodo; // asignamos a *p_inodo_dir el puntero al inodo que se indica en la entrada encontrada;
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