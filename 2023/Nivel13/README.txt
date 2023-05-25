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
            > MEJORA: *NO REALIZADA

    - ficheros_basico.h
        > MEJORA: uso de 'typedef union _inodo' en vez de 'struct inodo' <REALIZADA>

    - ficheros.c
        · int mi_write_f(unsigned int ninodo, const void *buf_original, unsigned int offset, unsigned int nbytes);
        · int mi_read_f(unsigned int ninodo, void *buf_original, unsigned int offset, unsigned int nbytes);
        · int mi_stat_f(unsigned int ninodo, struct STAT *p_stat);
        · int mi_chmod_f(unsigned int ninodo, unsigned char permisos);
        · int mi_truncar_f(unsigned int ninodo, unsigned int nbytes);

    - ficheros.h

    - directorios.c
        · int extraer_camino(const char *camino, char *inicial, char *final, char *tipo);
        · int buscar_entrada(const char *camino_parcial, unsigned int *p_inodo_dir, unsigned int *p_inodo, unsigned int *p_entrada, char reservar, unsigned char permisos);
            > MEJORA: *NO REALIZADA
        · void mostrar_error_buscar_entrada(int error);
        · int mi_creat(const char *camino, unsigned char permisos);
        · int mi_dir(const char *camino, char *buffer, char tipo);
        · int mi_chmod(const char *camino, unsigned char permisos);
        · int mi_stat(const char *camino, struct STAT *p_stat);
        · int mi_write(const char *camino, const void *buf, unsigned int offset, unsigned int nbytes);
        · int mi_read(const char *camino, void *buf, unsigned int offset, unsigned int nbytes);
        · int mi_link(const char *camino1, const char *camino2);
        · int mi_unlink(const char *camino);

    - directorios.h

    - mi_mkfs.c

    - leer_sf.c

    - leer.c

    - escribir.c

    - permitir.c

    - truncar.c

    - mi_mkdir.c

    - mi_touch.c *uso opcional*

    - mi_ls.c
        > MEJORAS:
            · Listar datos del inodo <REALIZADA>
            · Utilizar colores <REALIZADA>
            · Admitir comando mi_ls para ficheros <REALIZADA>
            · Distinguir entre formato simple o expandido *NO REALIZADA

    - mi_chmod.c

    - mi_stat.c

    - mi_escribir.c

    - mi_cat.c

    - mi_link.c

    - mi_rm.c

    - mi_rmdir.c *uso opcional*

    - semaforo_mutex_posix.c
        · sem_t initSem();
        · void deleteSem();
        · void signalSem(sem_t *sem);
        · void waitSem(sem_t *sem);

    - semaforo_mutex_posix.h

    - simulacion.c

    - simulacion.h

    - verificacion.c
        > MEJORA: leer entradas de golpe <REALIZADA>

    - verificacion.h

*********************************************************************************************************
******************************************FUNCIONALIDADES EXTRA******************************************
*********************************************************************************************************

    - mi_rn.c (Renombrar un fichero o directorio dentro de un mismo directorio) *NO REALIZADA

    - mi_mv.c (Mover un fichero o directorio a otro directorio) *NO REALIZADA

    - mi_cp_f.c (Copiar un fichero en otro directorio) *NO REALIZADA

    - mi_cp.c (Copiar un fichero o directorio) *NO REALIZADA

    - mi_rm_r.c (Borrar recursivamente un directorio no vacío) *NO REALIZADA

*********************************************************************************************************
*********************************************************************************************************
*********************************************************************************************************