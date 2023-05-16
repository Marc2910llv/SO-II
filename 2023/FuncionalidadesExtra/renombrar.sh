#ejecutar primero el script estructura.sh para montar el árbol de directorios de ejmplo

echo "$ ./mi_rn disco /dir1/dir12/fic122 fic121 #error"
./mi_rn disco /dir1/dir12/fic122 fic121
echo "$ ./mi_cat disco /dir2/dir22/fic221"
./mi_cat disco /dir2/dir22/fic221
echo "$ ./mi_rn disco /dir2/dir22/fic221 fic222"
./mi_rn disco /dir2/dir22/fic221 fic222
echo "$ ./mi_cat disco /dir2/dir22/fic221"
./mi_cat disco /dir2/dir22/fic221
echo "$ ./mi_cat disco /dir2/dir22/fic222"
./mi_cat disco /dir2/dir22/fic222
echo "$ ./mi_ls disco /dir2/dir22/"
./mi_ls disco /dir2/dir22/
echo "$ ./mi_cat disco /dir1/dir12/fic122"
./mi_cat disco /dir1/dir12/fic122
echo "$ ./mi_rn disco /dir1/dir12/fic122 fic123"
./mi_rn disco /dir1/dir12/fic122 fic123
echo "$ ./mi_cat disco /dir1/dir12/fic122"
./mi_cat disco /dir1/dir12/fic122
echo "$ ./mi_cat disco /dir1/dir12/fic123"
./mi_cat disco /dir1/dir12/fic123
echo "$ ./mi_ls disco /dir1/dir12/"
./mi_ls disco /dir1/dir12/
echo "$ ./mi_rn disco /dir2/dir23/ dir24"
./mi_rn disco /dir2/dir23/ dir24
echo "$ ./mi_ls disco /dir2/"
./mi_ls disco /dir2/

echo "$ ./leer_sf disco"
./leer_sf disco
echo
echo "###lo dejamos todo como antes!"
echo "$ ./mi_rn disco /dir1/dir12/fic123 fic122"
./mi_rn disco /dir1/dir12/fic123 fic122
echo "$ ./mi_rn disco /dir2/dir22/fic222 fic221"
./mi_rn disco /dir2/dir22/fic222 fic221
echo "$ ./mi_rn disco /dir2/dir24/ dir23"
./mi_rn disco /dir2/dir24/ dir23
echo "$ ./mi_ls disco /dir1/dir12/"
./mi_ls disco /dir1/dir12/
echo "$ ./mi_ls disco /dir2/dir22/"
./mi_ls disco /dir2/dir22/
echo "$ ./mi_ls disco /dir2/"
./mi_ls disco /dir2/

echo "$ ./leer_sf disco"
./leer_sf disco