CC=gcc
CFLAGS=-c -g -Wall -std=gnu99
#LDFLAGS=-pthread

SOURCES=mi_mkfs.c bloques.c #ﬁcheros_basico.c leer_sf.c ﬁcheros.c escribir.c leer.c
#truncar.c permitir.c directorios.c mi_mkdir.c mi_chmod.c mi_ls.c mi_link.c mi_escribir.c
#mi_cat.c mi_stat.c mi_rm.c semaforo_mutex_posix.c simulacion.c veriﬁcacion.c
LIBRARIES=bloques.o #ﬁcheros_basico.o ﬁcheros.o directorios.o
#semaforo_mutex_posix.o
INCLUDES=bloques.h #ﬁcheros_basico.h ﬁcheros.h directorios.h
#semaforo_mutex_posix.h simulacion.h
PROGRAMS=mi_mkfs #leer_sf escribir leer truncar permitir mi_mkdir mi_chmod mi_ls
#mi_link mi_escribir mi_cat mi_stat mi_rm  simulacion veriﬁcacion
OBJS=$(SOURCES:.c=.o)

all: $(OBJS) $(PROGRAMS)

$(PROGRAMS): $(LIBRARIES) $(INCLUDES)
    $(CC) $(LDFLAGS) $(LIBRARIES) $@.o -o $@

%.o: %.c $(INCLUDES)
    $(CC) $(CFLAGS) -o $@ -c $<

.PHONY: clean
clean:
    rm -rf *.o *~ $(PROGRAMS) disco* ext*