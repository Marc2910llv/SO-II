CC=gcc
CFLAGS= -c -g -Wall -std=gnu99
#LDFLAGS=-pthread
 
SOURCES=mi_mkfs.c bloques.c ficheros_basico.c leer_sf.c ficheros.c escribir.c leer.c truncar.c permitir.c 
LIBRARIES=bloques.o ficheros_basico.o ficheros.o 
INCLUDES=bloques.h ficheros_basico.h ficheros.h 
PROGRAMS=mi_mkfs leer_sf escribir leer truncar permitir 
 
all: $(OBJS) $(PROGRAMS)
 
$(PROGRAMS): $(LIBRARIES) $(INCLUDES)
	$(CC) $(LDFLAGS) $(LIBRARIES) $@.o -o $@
 
%.o: %.c $(INCLUDES)
	$(CC) $(CFLAGS) -o $@ -c $<
 
.PHONY: clean
clean:
	rm -rf *.o *~ $(PROGRAMS) disco* ext*  