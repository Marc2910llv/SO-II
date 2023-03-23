# Adelaida
# test5a.sh
clear
make clean
make
echo -e "\x1B[38;2;17;245;120m################################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_mkfs disco 100000\x1b[0m"
echo -e "\x1B[38;2;17;245;120m#inicializamos el sistema de ficheros con 100.000 bloques\x1b[0m"
./mi_mkfs disco 100000
echo
echo -e "\x1B[38;2;17;245;120m$ ./leer_sf disco\x1b[0m"
echo -e "\x1B[38;2;17;245;120m#mostramos solo el SB\x1b[0m"
./leer_sf disco
echo
echo -e "\x1B[38;2;17;245;120m################################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./escribir\x1b[0m"
echo -e "\x1B[38;2;17;245;120m#consultamos sintaxis comando\x1b[0m"
./escribir
echo
echo -e "\x1B[38;2;17;245;120m################################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./escribir disco "123456789" 0\x1b[0m"
echo -e "\x1B[38;2;17;245;120m#escribimos el texto “123456789” en los offsets  9000, 209000, 30725000, \x1b[0m"
echo -e "\x1B[38;2;17;245;120m#409605000 y 480000000 de un mismo inodo\x1b[0m"
./escribir disco "123456789" 0
echo
echo -e "\x1B[38;2;17;245;120m$ ./leer disco 1 > ext1.txt\x1b[0m"
echo -e "\x1B[38;2;17;245;120m#leemos el contenido del inodo 1 y lo direccionamos al fichero externo ext1.txt\x1b[0m"
echo
./leer disco 1 > ext1.txt
echo
echo -e "\x1B[38;2;17;245;120m$ ls -l ext1.txt\x1b[0m"
echo -e "\x1B[38;2;17;245;120m#comprobamos cuánto ocupa el fichero externo\x1b[0m"
echo -e "\x1B[38;2;17;245;120m#(ha de coincidir con el tamaño en bytes lógico del inodo y con los bytes leídos)\x1b[0m"
echo
ls -l ext1.txt
echo
echo -e "\x1B[38;2;17;245;120m################################################################################\x1b[0m\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./escribir disco "123456789" 1\x1b[0m"
echo -e "\x1B[38;2;17;245;120m#escribimos el texto “123456789” en los offsets 9000, 209000, 30725000, \x1b[0m"
echo -e "\x1B[38;2;17;245;120m#409605000 y 480000000, de inodos diferentes\x1b[0m"
./escribir disco "123456789" 1
echo
echo -e "\x1B[38;2;17;245;120m################################################################################\x1b[0m\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./leer disco 2 > ext2.txt\x1b[0m"
echo -e "\x1B[38;2;17;245;120m#leemos el contenido del inodo 2 (escrito en el offset 9000) y lo direccionamos\x1b[0m"
echo -e "\x1B[38;2;17;245;120m#al fichero externo ext2.txt\x1b[0m"
./leer disco 2 > ext2.txt
echo
echo -e "\x1B[38;2;17;245;120m$ ls -l ext2.txt\x1b[0m"
echo -e "\x1B[38;2;17;245;120m#comprobamos cuánto ocupa el fichero externo ext2.txt\x1b[0m"
echo -e "\x1B[38;2;17;245;120m#(ha de coincidir con el tamaño en bytes lógico del inodo 2 y con total_leidos)\x1b[0m"
ls -l ext2.txt
echo
echo -e "\x1B[38;2;17;245;120m$ cat ext2.txt\x1b[0m"
echo -e "\x1B[38;2;17;245;120m#usamos el comando cat del sistema para leer el contenido del fichero externo\x1b[0m"
cat ext2.txt
echo
echo
echo -e "\x1B[38;2;17;245;120m$ ./leer disco 2\x1b[0m"
echo -e "\x1B[38;2;17;245;120m#leemos el contenido de nuestro inodo 2\x1b[0m"
echo -e "\x1B[38;2;17;245;120m#(ha de contener lo mismo que el fichero externo ext2.txt)\x1b[0m"
./leer disco 2
echo
echo -e "\x1B[38;2;17;245;120m################################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./leer disco 5 > ext3.txt\x1b[0m"
echo -e "\x1B[38;2;17;245;120m#leemos todo el contenido del inodo 5 (escrito en el offset 409605000) y lo \x1b[0m"
echo -e "\x1B[38;2;17;245;120m#direccionamos al fichero externo ext3.txt\x1b[0m"
./leer disco 5 > ext3.txt
echo
echo -e "\x1B[38;2;17;245;120m$ ls -l ext3.txt\x1b[0m"
echo -e "\x1B[38;2;17;245;120m#comprobamos cuánto ocupa el fichero externo ext3.txt\x1b[0m"
echo -e "\x1B[38;2;17;245;120m#(ha de coincidir con el tamaño en bytes lógico del inodo 5 y con total_leidos)\x1b[0m"
ls -l ext3.txt
echo
echo -e "\x1B[38;2;17;245;120m$ cat ext3.txt\x1b[0m"
echo -e "\x1B[38;2;17;245;120m#usamos el comando cat del sistema para leer el contenido del fichero externo\x1b[0m"
cat ext3.txt
echo
echo
echo -e "\x1B[38;2;17;245;120m$ ./leer disco 5\x1b[0m"
echo -e "\x1B[38;2;17;245;120m#leemos el contenido de nuestro inodo 5\x1b[0m"
echo -e "\x1B[38;2;17;245;120m#(ha de contener lo mismo que el fichero externo ext3.txt)\x1b[0m"
./leer disco 5