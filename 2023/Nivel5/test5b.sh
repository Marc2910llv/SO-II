# Adelaida
# test5b.sh
clear
make clean
make
echo -e "\x1B[38;2;17;245;120m################################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_mkfs disco 100000\x1b[0m"
./mi_mkfs disco 100000
echo
echo -e "\x1B[38;2;17;245;120m################################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./escribir disco "$(cat texto2.txt)" 1\x1b[0m"
echo -e "\x1B[38;2;17;245;120m#escribimos el texto contenido en text2.txt en los offsets  9000, 209000, 30725000, \x1b[0m"
echo -e "\x1B[38;2;17;245;120m#409605000 y 480000000 de inodos diferentes\x1b[0m"
./escribir disco "$(cat texto2.txt)" 1
echo
echo -e "\x1B[38;2;17;245;120m################################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./leer disco 2 > ext4.txt\x1b[0m"
echo -e "\x1B[38;2;17;245;120m#leemos el contenido del inodo 2 (escrito en el offset 209000) y lo direccionamos\x1b[0m"
echo -e "\x1B[38;2;17;245;120m#al fichero externo ext4.txt\x1b[0m"
./leer disco 2 > ext4.txt
echo
echo -e "\x1B[38;2;17;245;120m$ ls -l ext4.txt\x1b[0m"
echo -e "\x1B[38;2;17;245;120m#comprobamos cuánto ocupa el fichero externo ext4.txt\x1b[0m"
echo -e "\x1B[38;2;17;245;120m#(ha de coincidir con el tamaño en bytes lógico del inodo 2 y con total_leidos)\x1b[0m"
ls -l ext4.txt
echo
echo -e "\x1B[38;2;17;245;120m$ cat ext4.txt\x1b[0m"
echo -e "\x1B[38;2;17;245;120m#usamos el cat del sistema para leer el contenido de nuestro fichero direccionado\x1b[0m"
echo -e "\x1B[38;2;17;245;120m#No hay que mostrar basura\x1b[0m"
cat ext4.txt
echo
echo
echo -e "\x1B[38;2;17;245;120m################################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./permitir\x1b[0m"
echo -e "\x1B[38;2;17;245;120m#mostramos sintaxis de permitir\x1b[0m"
./permitir
echo
echo -e "\x1B[38;2;17;245;120m################################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./permitir disco 2 0\x1b[0m"
echo -e "\x1B[38;2;17;245;120m#cambiamos permisos del inodo 2 a 0\x1b[0m"
./permitir disco 2 0
echo
echo -e "\x1B[38;2;17;245;120m$ ./leer disco 2\x1b[0m"
echo -e "\x1B[38;2;17;245;120m#intentamos leer inodo 2 con permisos=0\x1b[0m"
./leer disco 2