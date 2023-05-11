# Adelaida
# test10.sh
clear
make clean
make

echo -e "\x1B[38;2;17;245;120m##############################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m                          1ª parte\x1b[0m"
echo -e "\x1B[38;2;17;245;120m##############################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_mkfs disco 100000\x1b[0m"
./mi_mkfs disco 100000
echo
echo -e "\x1B[38;2;17;245;120m##############################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_mkdir disco 6 /dir1/\x1b[0m"
./mi_mkdir disco 6 /dir1/
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_mkdir disco 6 /dir1/dir11/\x1b[0m"
./mi_mkdir disco 6 /dir1/dir11/
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_touch disco 6 /dir1/dir11/fic1\x1b[0m"
./mi_touch disco 6 /dir1/dir11/fic1
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_escribir disco /dir1/dir11/fic1 hellooooooo 0\x1b[0m"
./mi_escribir disco /dir1/dir11/fic1 "hellooooooo" 0
echo
echo -e "\x1B[38;2;17;245;120m##############################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_mkdir disco 6 /dir2/\x1b[0m"
./mi_mkdir disco 6 /dir2/
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_mkdir disco 6 /dir2/dir21/\x1b[0m"
./mi_mkdir disco 6 /dir2/dir21/
echo
echo -e "\x1B[38;2;17;245;120m##############################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_link disco /dir1/dir11/fic1 /dir2/dir21/fic2\x1b[0m"
./mi_link disco /dir1/dir11/fic1 /dir2/dir21/fic2
echo
echo -e "\x1B[38;2;17;245;120m##############################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_cat disco /dir2/dir21/fic2 #ha de mostrar mismo contenido que /dir1/dir11/fic1\x1b[0m"
./mi_cat disco /dir2/dir21/fic2 
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_stat disco /dir1/dir11/fic1\x1b[0m"
./mi_stat disco /dir1/dir11/fic1 
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_stat disco /dir2/dir21/fic2 #han de tener el mismo inodo y 2 enlaces\x1b[0m"
./mi_stat disco /dir2/dir21/fic2
echo
echo -e "\x1B[38;2;17;245;120m##############################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_link disco /dir1/dir11/fic3 /di2/dir21/fic4  #camino1 ha de existir\x1b[0m"
./mi_link disco /dir1/dir11/fic3 /di2/dir21/fic4
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_touch disco 6 /dir1/dir11/fic3\x1b[0m"
./mi_touch disco 6 /dir1/dir11/fic3 
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_link disco /dir1/dir11/fic3 /dir2/dir21/fic4\x1b[0m"
./mi_link disco /dir1/dir11/fic3 /dir2/dir21/fic4
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_link disco /dir1/dir11/fic3 /dir2/dir21/fic5\x1b[0m"
./mi_link disco /dir1/dir11/fic3 /dir2/dir21/fic5
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_stat disco /dir1/dir11/fic3\x1b[0m"
./mi_stat disco /dir1/dir11/fic3
echo
echo -e "\x1B[38;2;17;245;120m##############################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_link disco /dir1/dir11/fic3 /dir2/dir21/fic2 #camino2 NO ha de existir\x1b[0m"
./mi_link disco /dir1/dir11/fic3 /dir2/dir21/fic2 #camino2 NO ha de existir 
echo
echo -e "\x1B[38;2;17;245;120m##############################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m                          2ª parte\x1b[0m"
echo -e "\x1B[38;2;17;245;120m##############################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_rmdir disco /dir2/dir21/ #o mi_rm\x1b[0m"
./mi_rmdir disco /dir2/dir21/ 
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_rm disco /dir2/dir21/fic2\x1b[0m"
./mi_rm disco /dir2/dir21/fic2
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_stat disco /dir1/dir11/fic1 #Hemos borrado 1 enlace\x1b[0m"
./mi_stat disco /dir1/dir11/fic1 #
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_rm disco /dir2/dir21/fic2\x1b[0m"
./mi_rm disco /dir2/dir21/fic2
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_rmdir disco /dir2/dir21/ #o mi_rm\x1b[0m"
./mi_rmdir disco /dir2/dir21/
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_ls disco /dir2/dir21/\x1b[0m"
./mi_ls disco /dir2/dir21/
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_rm disco /dir2/dir21/fic4\x1b[0m"
./mi_rm disco /dir2/dir21/fic4
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_rm disco /dir2/dir21/fic5\x1b[0m"
./mi_rm disco /dir2/dir21/fic5
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_rmdir disco /dir2/dir21/ #o mi_rm\x1b[0m"
./mi_rmdir disco /dir2/dir21/
echo
echo -e "\x1B[38;2;17;245;120m$ ./mi_ls disco /dir2/\x1b[0m"
./mi_ls disco /dir2/
echo


echo -e "\x1B[38;2;17;245;120m##############################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120mComprobamos que al crear 17 subdirectorios los bloques de datos del padre son 2\x1b[0m"
echo -e "\x1B[38;2;17;245;120m(en un bloque caben 16 entradas de directorio),\x1b[0m"
echo -e "\x1B[38;2;17;245;120m y que al eliminar un subdirectorio el directorio padre tiene 1 bloque de datos\x1b[0m"
echo -e "\x1B[38;2;17;245;120m##############################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_mkdir disco /d1/\x1b[0m"
./mi_mkdir disco 6 /d1/
echo
echo -e "\x1B[38;2;17;245;120m##############################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120mcreamos 17 subdirectorios sd0, sd1..., sd16 en d1\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ for i in \$(seq 0 16)\x1b[0m"
echo -e "\x1B[38;2;17;245;120m> do\x1b[0m"
echo -e "\x1B[38;2;17;245;120m> ./mi_mkdir disco 6 /d1/sd\$i/\x1b[0m"
echo -e "\x1B[38;2;17;245;120m> done\x1b[0m"
for i in $(seq 0 16)
do
    ./mi_mkdir disco 6 /d1/sd$i/
done
echo
echo -e "\x1B[38;2;17;245;120mMostramos la metainformacion del directorio para ver que tiene 2 bloques de datos\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_stat disco /d1/\x1b[0m"
./mi_stat disco /d1/
echo
echo -e "\x1B[38;2;17;245;120mListamos el directorio para ver sus subdirectorios\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_ls disco /d1/\x1b[0m"
./mi_ls disco /d1/
echo
echo -e "\x1B[38;2;17;245;120m##############################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120mEliminamos el subdirectorio sd3 de d1\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_rmdir disco /d1/sd3/\x1b[0m"
./mi_rmdir disco /d1/sd3/
echo
echo -e "\x1B[38;2;17;245;120mMostramos la metainformacion de d1 para ver que ahora tiene 1 bloque de datos\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_stat disco /d1/\x1b[0m"
./mi_stat disco /d1/
echo
echo -e "\x1B[38;2;17;245;120mVolvemos a listar el directorio para ver que se ha eliminado un subdirectorio\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_ls disco /d1/\x1b[0m"
./mi_ls disco /d1/