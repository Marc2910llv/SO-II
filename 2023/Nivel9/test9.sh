# Adelaida
# test9.sh
clear
make clean
make
echo -e "\x1B[38;2;17;245;120m##############################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_mkfs disco 100000\x1b[0m"
./mi_mkfs disco 100000
echo
echo -e "\x1B[38;2;17;245;120m##############################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_touch disco 6 /fichero\x1b[0m"
./mi_touch disco 6 /fichero
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_ls disco /\x1b[0m"
./mi_ls disco /
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_escribir  disco /fichero "hola que tal" 5120\x1b[0m"
./mi_escribir  disco /fichero "hola que tal" 5120
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_ls disco /\x1b[0m"
./mi_ls disco /
echo
echo -e "\x1B[38;2;17;245;120m$ ./leer_sf disco\x1b[0m"
./leer_sf disco
echo -e "\x1B[38;2;17;245;120m##############################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_chmod disco 4 /fichero #r--\x1b[0m"
./mi_chmod disco 4 /fichero
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_escribir  disco /fichero "estoy estupendamente" 256000\x1b[0m"
./mi_escribir  disco /fichero "estoy estupendamente" 256000
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_ls disco /fichero #si no habéis implementado mi_ls para ficheros eliminar esta línea\x1b[0m"
./mi_ls disco /fichero
echo
echo -e "\x1B[38;2;17;245;120m##############################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_mkdir disco 6 /dir1/\x1b[0m"
./mi_mkdir disco 6 /dir1/
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_touch disco 6 /dir1/fic1\x1b[0m"
./mi_touch disco 6 /dir1/fic1
echo
echo -e "\x1B[38;2;17;245;120m./mi_escribir disco /dir1/fic1 hola1  256000\x1b[0m"
./mi_escribir disco /dir1/fic1 hola1  256000
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_stat disco /dir1/fic1\x1b[0m"
./mi_stat disco /dir1/fic1
echo
echo -e "\x1B[38;2;17;245;120m##############################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ sleep 1 #esperamos un poco para observar los sellos de tiempo\x1b[0m"
sleep 1 
echo
echo -e "\x1B[38;2;17;245;120m##############################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_escribir disco /dir1/fic1 hola2  5120 #no cambia tamenBytesLog pero sí mtime y ctime (ocupamos 1 bloque más)\x1b[0m"
./mi_escribir disco /dir1/fic1 hola2  5120 
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_stat disco /dir1/fic1\x1b[0m"
./mi_stat disco /dir1/fic1
echo
echo -e "\x1B[38;2;17;245;120m##############################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ sleep 1 #esperamos un poco para observar los sellos de tiempo\x1b[0m"
sleep 1 
echo
echo -e "\x1B[38;2;17;245;120m##############################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_escribir disco /dir1/fic1 hola3  5200 #mismo bloque que offset 5120, cambia mtime pero no ctime\x1b[0m"
./mi_escribir disco /dir1/fic1 hola3  5200 
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_stat disco /dir1/fic1\x1b[0m"
./mi_stat disco /dir1/fic1
echo
echo -e "\x1B[38;2;17;245;120m##############################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120msleep 1 #esperamos un poco para observar los sellos de tiempo\x1b[0m"
sleep 1
echo
echo -e "\x1B[38;2;17;245;120m##############################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m./mi_escribir disco /dir1/fic1 hola4 256010 #cambia tamEnBytesLog, mtime y ctime\x1b[0m"
./mi_escribir disco /dir1/fic1 hola4 256010
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_stat disco /dir1/fic1\x1b[0m"
./mi_stat disco /dir1/fic1
echo
echo -e "\x1B[38;2;17;245;120m##############################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m# Comprobación de la caché de directorios (1 entrada para L y otra para E)\x1b[0m"
echo -e "\x1B[38;2;17;245;120m##############################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_touch disco 6 /dir1/fic2\x1b[0m"
./mi_touch disco 6 /dir1/fic2
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_escribir disco /dir1/fic2 "$(cat texto2.txt)" 1000\x1b[0m"
./mi_escribir disco /dir1/fic2 "$(cat texto2.txt)" 1000
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_cat disco /dir1/fic2 #tambuffer=BLOCKSIZE * 4\x1b[0m"
./mi_cat disco /dir1/fic2 
echo
echo
echo -e "\x1B[38;2;17;245;120m##############################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_escribir disco /dir1/fic2 \"******************************\" 10000\x1b[0m"
./mi_escribir disco /dir1/fic2 "******************************" 10000
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_ls disco /dir1/\x1b[0m"
./mi_ls disco /dir1/
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_cat disco /dir1/fic2 #tambuffer=BLOCKSIZE * 4\x1b[0m"
./mi_cat disco /dir1/fic2 
echo
echo
echo -e "\x1B[38;2;17;245;120m##############################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_touch disco 6 /dir1/fic3\x1b[0m"
./mi_touch disco 6 /dir1/fic3
echo -e "\x1B[38;2;17;245;120m$ ./mi_escribir_varios disco /dir1/fic3 \"--texto repetido en 10 bloques--\" 0\x1b[0m"
./mi_escribir_varios disco /dir1/fic3 "--texto repetido en 10 bloques--" 0
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_stat disco /dir1/fic3\x1b[0m"
./mi_stat disco /dir1/fic3
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_cat disco /dir1/fic3 #tambuffer=BLOCKSIZE * 4\x1b[0m"
./mi_cat disco /dir1/fic3 
echo