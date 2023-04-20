# Adelaida
# test8.sh
clear
make clean
make

echo -e "\x1B[38;2;17;245;120m################################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_mkfs disco 100000\x1b[0m"
./mi_mkfs disco 100000
echo
echo -e "\x1B[38;2;17;245;120m################################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_mkdir #comprobar sintaxis\x1b[0m"
./mi_mkdir 
echo
echo -e "\x1B[38;2;17;245;120m################################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_mkdir disco 7 / #no ha de dejar crear la raíz al usuario\x1b[0m"
./mi_mkdir disco 7 /
echo
echo -e "\x1B[38;2;17;245;120m################################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_mkdir disco 6 dir1/\x1b[0m"
./mi_mkdir disco 6 dir1/
echo
echo -e "\x1B[38;2;17;245;120m################################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_mkdir disco 6 /dir1/   #permiso lectura/escritura: rw-\x1b[0m"
./mi_mkdir disco 6 /dir1/ 
echo  
echo -e "\x1B[38;2;17;245;120m$ ./mi_mkdir disco 6 /dir1/dir11/   #permiso lectura/escritura: rw-\x1b[0m"
./mi_mkdir disco 6 /dir1/dir11/  
echo
echo -e "\x1B[38;2;17;245;120m################################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_chmod #comprobar sintaxis\x1b[0m"
./mi_chmod 
echo
echo -e "\x1B[38;2;17;245;120m################################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_chmod disco 1 /dir1/dir11/   #permiso ejecución: --x\x1b[0m"
./mi_chmod disco 1 /dir1/dir11/  
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_touch disco 6 /dir1/dir11/fic111   #Error: Permiso denegado de lectura.\x1b[0m"
./mi_touch disco 6 /dir1/dir11/fic111  
echo
echo -e "\x1B[38;2;17;245;120m################################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m./mi_chmod disco 2 /dir1/dir11/  #permiso escritura -w-\x1b[0m"
./mi_chmod disco 2 /dir1/dir11/  
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_touch disco 6 /dir1/dir11/fic111   #Error: Permiso denegado de lectura.\x1b[0m"
./mi_touch disco 6 /dir1/dir11/fic111  
echo
echo -e "\x1B[38;2;17;245;120m################################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_chmod disco 6 /dir1/dir11/   #permiso lectura/escritura: rw-\x1b[0m"
./mi_chmod disco 6 /dir1/dir11/   
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_touch disco 6 /dir1/dir11/fic111   #permiso lectura/escritura: rw-\x1b[0m"
./mi_touch disco 6 /dir1/dir11/fic111  
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_touch disco 6 /dir1/dir11/fic112   #permiso lectura/escritura: rw-\x1b[0m"
./mi_touch disco 6 /dir1/dir11/fic112  
echo
echo -e "\x1B[38;2;17;245;120m################################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_ls disco /\x1b[0m"
./mi_ls disco /
echo
echo -e "\x1B[38;2;17;245;120m################################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_stat disco /dir1/\x1b[0m"
./mi_stat disco /dir1/
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_ls disco /dir1/ \x1b[0m"
./mi_ls disco /dir1/ 
echo
echo -e "\x1B[38;2;17;245;120m################################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_stat disco /dir1/dir11/\x1b[0m"
./mi_stat disco /dir1/dir11/
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_ls disco /dir1/dir11/\x1b[0m"
./mi_ls disco /dir1/dir11/
echo
echo -e "\x1B[38;2;17;245;120m################################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m./mi_ls disco /dir1/dir12/ #Error: No existe el archivo o el directorio.\x1b[0m"
./mi_ls disco /dir1/dir12/
echo
echo -e "\x1B[38;2;17;245;120m################################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_touch disco 6 /dir1/dir11/fic111 #Error: El archivo ya existe.\x1b[0m"
./mi_touch disco 6 /dir1/dir11/fic111 
echo 
echo -e "\x1B[38;2;17;245;120m################################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_mkdir disco 6 /dir1/dir11/fic111/dir12/ #Error: No es un directorio.\x1b[0m"
./mi_mkdir disco 6 /dir1/dir11/fic111/dir12/
echo
echo -e "\x1B[38;2;17;245;120m################################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_touch disco 6 /dir1/dir11/dir12/fic111 #Error: No existe algún directorio intermedio.\x1b[0m"
./mi_touch disco 6 /dir1/dir11/dir12/fic111
echo
echo -e "\x1B[38;2;17;245;120m################################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_mkdir disco 9 /dir2/ #Error: modo inválido: <<9>>\x1b[0m"
./mi_mkdir disco 9 /dir2/