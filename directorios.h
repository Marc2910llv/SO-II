// Pere Joan Vives Morey
// Marc Llobera Villalonga
// Carlos Lozano Alemañy
#define TAMNOMBRE 60 // tamaño del nombre de directorio o fichero, en Ext2 = 256
struct entrada
{
    char nombre[TAMNOMBRE];
    unsigned int ninodo;
};

int extraer_camino(const char *camino, char *inicial, char *final, char *tipo);
int buscar_entrada(const char *camino_parcial, unsigned int *p_inodo_dir, unsigned int *p_inodo, unsigned int *p_entrada, char reservar, unsigned char permisos);
