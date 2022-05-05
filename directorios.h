// Pere Joan Vives Morey
// Marc Llobera Villalonga
// Carlos Lozano Alemañy
#include "ficheros.h"
#define TAMNOMBRE 60 // tamaño del nombre de directorio o fichero, en Ext2 = 256
struct entrada
{
    char nombre[TAMNOMBRE];
    unsigned int ninodo;
};

#define ERROR_CAMINO_INCORRECTO -1
#define ERROR_PERMISO_LECTURA -2
#define ERROR_NO_EXISTE_ENTRADA_CONSULTA -3
#define ERROR_NO_EXISTE_DIRECTORIO_INTERMEDIO -4
#define ERROR_PERMISO_ESCRITURA -5
#define ERROR_ENTRADA_YA_EXISTENTE -6
#define ERROR_NO_SE_PUEDE_CREAR_ENTRADA_EN_UN_FICHERO -7

int extraer_camino(const char *camino, char *inicial, char *final, char *tipo);
int buscar_entrada(const char *camino_parcial, unsigned int *p_inodo_dir, unsigned int *p_inodo, unsigned int *p_entrada, char reservar, unsigned char permisos);
void mostrar_error_buscar_entrada(int error);
int mi_creat(const char *camino, unsigned char permisos);
int mi_dir(const char *camino, char *buffer);
int mi_chmod(const char *camino, unsigned char permisos);
int mi_stat(const char *camino, struct STAT *p_stat);