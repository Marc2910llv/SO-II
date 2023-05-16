echo "$./leer_sf disco"
./leer_sf disco
echo "$./mi_ls disco /dir2/dir22/"
./mi_ls disco /dir2/dir22/
echo "$./mi_mv disco /dir2/dir22/fic221 /dir1/fic11 #error destino"
./mi_mv disco /dir2/dir22/fic221 /dir1/fic11
echo "$./mi_ls disco /dir2/dir21/"
./mi_ls disco /dir2/dir21/
echo "$./mi_mv disco /dir2/dir22/fic221 /dir2/dir21/ #movemos un fichero"
./mi_mv disco /dir2/dir22/fic221 /dir2/dir21/
echo "$./mi_ls disco /dir2/dir22/"
./mi_ls disco /dir2/dir22/
echo "$./mi_ls disco /dir2/dir21/"
./mi_ls disco /dir2/dir21/
echo "$./leer_sf disco #Un bloque más libre"
./leer_sf disco
echo "$./mi_ls disco /"
./mi_ls disco /
echo "$./mi_ls disco /dir1/"
./mi_ls disco /dir1/
echo "$./mi_mv disco /dir3/ /dir1/ #movemos un directorio"
./mi_mv disco /dir3/ /dir1/
echo "$./mi_ls disco /"
./mi_ls disco /
echo "$./mi_ls disco /dir1/"
./mi_ls disco /dir1/
echo "$./leer_sf disco"
./leer_sf disco
echo "$./mi_mv disco /dir1/ /dir3/ #error"
./mi_mv disco /dir1/ /dir3/

echo "###lo dejamos todo como antes!"
echo "$./mi_mv disco /dir2/dir21/fic221 /dir2/dir22/"
./mi_mv disco /dir2/dir21/fic221 /dir2/dir22/
echo "$./mi_ls disco /dir2/dir22/"
./mi_ls disco /dir2/dir22/
echo "$./mi_ls disco /dir2/dir21/"
./mi_ls disco /dir2/dir21/
echo "$./leer_sf disco #Un bloque menos libre"
./leer_sf disco
echo "$./mi_mv disco /dir1/dir3/ /"
./mi_mv disco /dir1/dir3/ /
echo "$./mi_ls disco /"
./mi_ls disco /
echo "$./mi_ls disco /dir1/"
./mi_ls disco /dir1/
echo "$./leer_sf disco"
./leer_sf disco