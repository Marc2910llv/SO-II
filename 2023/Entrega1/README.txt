Autor:
    Marc Llobera Villalonga

Ficheros:
    - bloques.c
        · int bmount(const char *camino);
        · int bumount();
        · int bwrite(unsigned int nbloque, const void *buf);
        · int bread(unsigned int nbloque, void *buf);
    - bloques.h
    - ficheros_basico.c
        · int tamMB(unsigned int nbloques);
        · int tamAI(unsigned int ninodos);
        · int initSB(unsigned int nbloques, unsigned int ninodos);
        · int initMB();
        · int initAI();
        · int escribir_bit(unsigned int nbloque, unsigned int bit);
        · char leer_bit(unsigned int nbloque);
        · int reservar_bloque();
        · int liberar_bloque(unsigned int nbloque);
        · int escribir_inodo(unsigned int ninodo, union _inodo inodo);
        · int leer_inodo(unsigned int ninodo, union _inodo *inodo);
        · int reservar_inodo(unsigned char tipo, unsigned char permisos);
        · int obtener_nRangoBL(union _inodo *inodo, unsigned int nblogico, unsigned int *ptr);
        · int obtener_indice(unsigned int nblogico, int nivel_punteros);
        · int traducir_bloque_inodo(unsigned int ninodo, unsigned int nblogico, char reservar);
        · int liberar_inodo(unsigned int ninodo);
        · int liberar_bloques_inodo(unsigned int primerBL, union _inodo *inodo);
            * MEJORA: x
    - ficheros_basico.h
        * MEJORA: uso de 'typedef union _inodo' en vez de 'struct inodo'
    - ficheros.c
        · int mi_write_f(unsigned int ninodo, const void *buf_original, unsigned int offset, unsigned int nbytes);
        · int mi_read_f(unsigned int ninodo, void *buf_original, unsigned int offset, unsigned int nbytes);
        · int mi_stat_f(unsigned int ninodo, struct STAT *p_stat);
        · int mi_chmod_f(unsigned int ninodo, unsigned char permisos);
        · int mi_truncar_f(unsigned int ninodo, unsigned int nbytes);
    - ficheros.h
    - mi_mkfs.c
    - leer_sf.c
    - leer.c
    - escribir.c
    - permitir.c
    - truncar.c
