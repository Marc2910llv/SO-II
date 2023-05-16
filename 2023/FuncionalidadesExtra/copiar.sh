echo "$ #Copia de un fichero en un directorio"
echo "$./mi_cp_f disco    /dir1/dir12/fic121     /dir3/"
./mi_cp_f disco    /dir1/dir12/fic121     /dir3/
echo "$ ./leer_sf disco"
./leer_sf disco

echo "$ ./mi_stat disco /dir1/dir12/fic121"
./mi_stat disco /dir1/dir12/fic121
echo "$ ./mi_stat disco /dir3/fic121 #ha de ser igual salvo sellos de tiempo"
./mi_stat disco /dir3/fic121

echo "$ ./mi_cat disco /dir1/dir12/fic121"
./mi_cat disco /dir1/dir12/fic121
echo "$ ./mi_cat disco /dir3/fic121"
./mi_cat disco /dir3/fic121

echo "######################################################################"
echo "$ #Copia de un directorio en otro"
echo "$ ./mi_cp disco /dir2/ /dir3/dir32/"
./mi_cp disco /dir2/ /dir3/dir32/
echo "$ ./leer_sf disco"
./leer_sf disco

echo "$ ./mi_ls disco /dir2/"
./mi_ls disco /dir2/
echo "$ ./mi_ls disco /dir3/dir32/dir2/"
./mi_ls disco /dir3/dir32/dir2/

echo "$ ./mi_ls disco /dir2/dir21/"
./mi_ls disco /dir2/dir21/
echo "$ ./mi_ls disco /dir3/dir32/dir2/dir21/"
./mi_ls disco /dir3/dir32/dir2/dir21/

echo "$ ./mi_ls disco /dir2/dir22/"
./mi_ls disco /dir2/dir22/
echo "$ ./mi_ls disco /dir3/dir32/dir2/dir22/"
./mi_ls disco /dir3/dir32/dir2/dir22/

echo "$ ./mi_stat disco /dir2/dir21/fic211"
./mi_stat disco /dir2/dir21/fic211
echo "$ ./mi_stat disco /dir3/dir32/dir2/dir21/fic211"
./mi_stat disco /dir3/dir32/dir2/dir21/fic211

echo "$ ./mi_stat disco /dir2/dir22/fic221"
./mi_stat disco /dir2/dir22/fic221
echo "$ ./mi_stat disco /dir3/dir32/dir2/dir22/fic221"
./mi_stat disco /dir3/dir32/dir2/dir22/fic221

echo "$ ./mi_cat disco /dir2/dir21/fic211"
./mi_cat disco /dir2/dir21/fic211
echo "$ ./mi_cat disco /dir3/dir32/dir2/dir21/fic211"
./mi_cat disco /dir3/dir32/dir2/dir21/fic211

echo "$ ./mi_cat disco /dir2/dir22/fic221"
./mi_cat disco /dir2/dir22/fic221
echo "$ ./mi_cat disco /dir3/dir32/dir2/dir22/fic221"
./mi_cat disco /dir3/dir32/dir2/dir22/fic221