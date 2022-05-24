make clean
make
echo -e "\x1B[38;2;17;245;120m$ ./mi_mkfs disco 100000\x1b[0m"
./mi_mkfs disco 100000
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./leer_sf disco\x1b[0m"
./leer_sf disco
echo
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120mNIVEL 8: CREAR DIRECTORIOS Y FICHEROS, PERMISOS, STAT, LISTAR\x1b[0m"
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_mkdir #comprobar sintaxis\x1b[0m"
./mi_mkdir 
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_mkdir disco 7 / #no ha de dejar crear la raíz al usuario\x1b[0m"
./mi_mkdir disco 7 /
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_mkdir disco 6 dir1/\x1b[0m"
./mi_mkdir disco 6 dir1/
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_mkdir disco 6 /dir1/\x1b[0m"  
./mi_mkdir disco 6 /dir1/   
echo -e "\x1B[38;2;17;245;120m$ ./mi_mkdir disco 6 /dir1/dir11/\x1b[0m"
./mi_mkdir disco 6 /dir1/dir11/  
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_chmod #comprobar sintaxis\x1b[0m"
./mi_chmod 
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_chmod disco 1 /dir1/dir11/ #cambio a permiso ejecución\x1b[0m"
./mi_chmod disco 1 /dir1/dir11/  
echo -e "\x1B[38;2;17;245;120m$ ./mi_touch disco 6 /dir1/dir11/fic111 #Error: Permiso denegado de lectura.\x1b[0m"
./mi_touch disco 6 /dir1/dir11/fic111  
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_chmod disco 2 /dir1/dir11/ #cambio a permiso escritura\x1b[0m"
./mi_chmod disco 2 /dir1/dir11/  
echo -e "\x1B[38;2;17;245;120m$ ./mi_touch disco 6 /dir1/dir11/fic111 #Error: Permiso denegado de lectura.\x1b[0m"
./mi_touch disco 6 /dir1/dir11/fic111  
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_chmod disco 6 /dir1/dir11/ #cambio a permiso lectura/escritura\x1b[0m"
./mi_chmod disco 6 /dir1/dir11/   
echo -e "\x1B[38;2;17;245;120m$ ./mi_touch disco 6 /dir1/dir11/fic111\x1b[0m"
./mi_touch disco 6 /dir1/dir11/fic111  
echo -e "\x1B[38;2;17;245;120m$ ./mi_touch disco 6 /dir1/dir11/fic112\x1b[0m"
./mi_touch disco 6 /dir1/dir11/fic112  
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo
echo -e "\x1B[38;2;17;245;120m"
echo "                  /"
echo "                  |"
echo "                dir1"
echo "                  |"
echo "                dir11"
echo "               /     \\"
echo "           fic111   fic112"
echo -e "\x1b[0m"
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_ls disco /\x1b[0m"
./mi_ls disco /
echo -e "\x1B[38;2;17;245;120m$ ./mi_stat disco /dir1/\x1b[0m"
./mi_stat disco /dir1/
echo -e "\x1B[38;2;17;245;120m$ ./mi_ls disco /dir1/\x1b[0m"
./mi_ls disco /dir1/ 
echo -e "\x1B[38;2;17;245;120m$ ./mi_stat disco /dir1/dir11/\x1b[0m"
./mi_stat disco /dir1/dir11/
echo -e "\x1B[38;2;17;245;120m$ ./mi_ls disco /dir1/dir11/\x1b[0m"
./mi_ls disco /dir1/dir11/
echo -e "\x1B[38;2;17;245;120m$ ./mi_ls disco /dir1/dir12/ #Error: No existe el archivo o el directorio.\x1b[0m"
./mi_ls disco /dir1/dir12/
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_touch disco 6 /dir1/dir11/fic111 #Error: El archivo ya existe.\x1b[0m"
./mi_touch disco 6 /dir1/dir11/fic111  
echo -e "\x1B[38;2;17;245;120m$ ./mi_mkdir disco 6 /dir1/dir11/fic111/dir12/ #Error: No es un directorio.\x1b[0m"
./mi_mkdir disco 6 /dir1/dir11/fic111/dir12/
echo -e "\x1B[38;2;17;245;120m$ ./mi_touch disco 6 /dir1/dir11/dir12/fic111 #Error: No existe algún directorio intermedio.\x1b[0m"
./mi_touch disco 6 /dir1/dir11/dir12/fic111
echo -e "\x1B[38;2;17;245;120m$ ./mi_mkdir disco 9 /dir2/ #Error: modo inválido: <<9>>\x1b[0m"
./mi_mkdir disco 9 /dir2/
echo

echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120mNIVEL 9: LEER Y ESCRIBIR\x1b[0m"
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./leer_sf disco\x1b[0m"
./leer_sf disco
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_escribir  disco /dir1/dir11/fic111 \"$(cat texto2.txt)\" 0 #⊂ BL 0 ∈ D0\x1b[0m"
echo -e "\x1B[38;2;17;245;120m#reservaremos 5 bloques de datos (3 escritos completos y 2 parciales)\x1b[0m"
./mi_escribir  disco /dir1/dir11/fic111 "$(cat texto2.txt)" 0
echo -e "\x1B[38;2;17;245;120m$ ./leer_sf disco\x1b[0m"
./leer_sf disco
echo -e "\x1B[38;2;17;245;120m$ ./mi_cat  disco /dir1/dir11/fic111>ext1.txt\x1b[0m"
./mi_cat  disco /dir1/dir11/fic111>ext1.txt
echo -e "\x1B[38;2;17;245;120m$ ls -l ext1.txt #comprobamos el tamaño del fichero externo\x1b[0m" 
ls -l ext1.txt
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_chmod disco 4 /dir1/dir11/fic111 #cambio a permiso lectura\x1b[0m"
./mi_chmod disco 4 /dir1/dir11/fic111
echo -e "\x1B[38;2;17;245;120m$ ./mi_escribir  disco /dir1/dir11/fic111 \"lo que sea\" 209000 #⊂ BL 204 ∈ I0\x1b[0m"
./mi_escribir  disco /dir1/dir11/fic111 "lo que sea" 209000
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_ls disco /dir1/dir11/\x1b[0m"
./mi_ls disco /dir1/dir11/
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_escribir disco /dir1/dir11/fic112 \"hola1\" 209000 #⊂ BL 204 ∈ I0\x1b[0m"
echo -e "\x1B[38;2;17;245;120m#modifica tamEnBytesLog, mtime y ctime, y reservamos 1 bloque datos y 1 de punteros\x1b[0m"
./mi_escribir disco /dir1/dir11/fic112 "hola1"  209000
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_stat disco /dir1/dir11/fic112\x1b[0m"
./mi_stat disco /dir1/dir11/fic112
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ sleep 2 #esperamos para distanciar los sellos de tiempo\x1b[0m"
sleep 2 
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_escribir disco /dir1/dir11/fic112 \"hola2\" 9000 #⊂ BL 8 ∈ D8\x1b[0m"
echo -e "\x1B[38;2;17;245;120m#no cambia tamenBytesLog pero sí mtime y ctime (ocupamos 1 bloque más de datos)\x1b[0m"
./mi_escribir disco /dir1/dir11/fic112 "hola2" 9000 
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_stat disco /dir1/dir11/fic112\x1b[0m"
./mi_stat disco /dir1/dir11/fic112
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ sleep 2 #esperamos para distanciar los sellos de tiempo\x1b[0m"
sleep 2 
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_escribir disco /dir1/dir11/fic112 \"hola3\" 9100 #⊂ BL 8 ∈ D8\x1b[0m"
echo -e "\x1B[38;2;17;245;120m#mismo bloque que offset 9000, cambia mtime pero no ctime\x1b[0m"
./mi_escribir disco /dir1/dir11/fic112 "hola3"  9100
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_stat disco /dir1/dir11/fic112\x1b[0m"
./mi_stat disco /dir1/dir11/fic112
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ sleep 2 #esperamos para distanciar los sellos de tiempo\x1b[0m"
sleep 2
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_escribir disco /dir1/dir11/fic112 \"hola4\" 275000 #⊂ BL 268 ∈ I1\x1b[0m" 
echo -e "\x1B[38;2;17;245;120m#cambia tamEnBytesLog, mtime y ctime, y reservamos 1 bloque datos y 2 de punteros\x1b[0m"
./mi_escribir disco /dir1/dir11/fic112 "hola4" 275000
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_stat /dir1/dir11/fic112 \x1b[0m"
./mi_stat disco /dir1/dir11/fic112 
echo -e "\x1B[38;2;17;245;120m$ ./mi_cat /dir1/dir11/fic112 \x1b[0m"
./mi_cat disco /dir1/dir11/fic112 
echo

echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120mNIVEL 10: ENLACES Y BORRADO\x1b[0m"
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m"
echo "                  /"
echo "                  |"
echo "                dir1"
echo "                  |"
echo "                dir11"
echo "               /     \\"
echo "           fic111   fic112"
echo -e "\x1b[0m"
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_ls disco /dir1/dir11/\x1b[0m"
./mi_ls disco /dir1/dir11/
echo -e "\x1B[38;2;17;245;120m$ ./mi_touch disco 6 /dir1/dir11/fic113\x1b[0m"
./mi_touch disco 6 /dir1/dir11/fic113
echo -e "\x1B[38;2;17;245;120m$ ./mi_escribir disco /dir1/dir11/fic113 \"hellooooooo\" 409605000 #⊂ BL 400.004 ∈ I2\x1b[0m"
./mi_escribir disco /dir1/dir11/fic113 "hellooooooo" 409605000
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_mkdir disco 6 /dir2/\x1b[0m"
./mi_mkdir disco 6 /dir2/
echo -e "\x1B[38;2;17;245;120m$ ./mi_mkdir disco 6 /dir2/dir21/\x1b[0m"
./mi_mkdir disco 6 /dir2/dir21/
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_link disco /dir1/dir11/fic113 /dir2/dir21/fic211\x1b[0m"
./mi_link disco /dir1/dir11/fic113 /dir2/dir21/fic211
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m"
echo "                                 /"
echo "                      ___________|__________" 
echo "                     /                      \\"
echo "                   dir1                   dir2"
echo "                     |                      |"
echo "                   dir11                  dir21"
echo "                  /  |  \                   |"
echo "                 /   |   \                  |"
echo "           fic111 fic112 fic113 <-------- fic211"
echo -e "\x1b[0m"
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_cat disco /dir2/dir21/fic211\x1b[0m" 
echo -e "\x1B[38;2;17;245;120m#ha de mostrar mismo contenido que /dir1/dir11/fic113\x1b[0m"
./mi_cat disco /dir2/dir21/fic211 
echo -e "\x1B[38;2;17;245;120m$ ./mi_stat disco /dir1/dir11/fic113\x1b[0m"
./mi_stat disco /dir1/dir11/fic113 
echo -e "\x1B[38;2;17;245;120m$ ./mi_stat disco /dir2/dir21/fic211 #han de tener el mismo inodo y 2 enlaces\x1b[0m"
./mi_stat disco /dir2/dir21/fic211
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_link disco /dir1/dir11/fic114 /di2/dir21/fic212\x1b[0m" 
echo -e "\x1B[38;2;17;245;120m#camino1 ha de existir\x1b[0m"
./mi_link disco /dir1/dir11/fic14 /di2/dir21/fic212  #camino1 ha de existir 
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_link disco /dir1/dir11/fic113 /dir2/dir21/fic211\x1b[0m"
echo -e "\x1B[38;2;17;245;120m#camino2 NO ha de existir\x1b[0m"
./mi_link disco /dir1/dir11/fic113 /dir2/dir21/fic211 #camino2 NO ha de existir 
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_rmdir disco /dir2/dir21/ #o mi_rm\x1b[0m"
echo -e "\x1B[38;2;17;245;120m#Error: El directorio /dir2/dir21/ no está vacío\x1b[0m"
./mi_rmdir disco /dir2/dir21/ 
echo -e "\x1B[38;2;17;245;120m$ ./mi_rm disco /dir2/dir21/fic211\x1b[0m"
./mi_rm disco /dir2/dir21/fic211
echo -e "\x1B[38;2;17;245;120m$ ./mi_stat disco /dir1/dir11/fic113 #Hemos borrado 1 enlace\x1b[0m"
./mi_stat disco /dir1/dir11/fic113 #Hemos borrado 1 enlace
echo -e "\x1B[38;2;17;245;120m$ ./leer_sf disco\x1b[0m"
./leer_sf disco
echo -e "\x1B[38;2;17;245;120m$ ./mi_rm disco /dir1/dir11/fic113 #Ahora sí se liberará el inodo y sus bloques\x1b[0m"
./mi_rm disco /dir1/dir11/fic113
echo -e "\x1B[38;2;17;245;120m$ ./leer_sf disco\x1b[0m"
./leer_sf disco
echo -e "\x1B[38;2;17;245;120m$ ./mi_rm disco /dir2/dir21/fic211 #ya no existe\x1b[0m"
./mi_rm disco /dir2/dir21/fic211
echo -e "\x1B[38;2;17;245;120m$ ./mi_rmdir disco /dir2/dir21/ #o mi_rm\x1b[0m"
./mi_rmdir disco /dir2/dir21/
echo -e "\x1B[38;2;17;245;120m$ ./mi_ls disco /dir2/\x1b[0m"
./mi_ls disco /dir2/

echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m"
echo "                                 /"
echo "                      ___________|_________" 
echo "                     /                     \\"
echo "                   dir1                   dir2"
echo "                     |"
echo "                   dir11"
echo "                  /     \\"
echo "              fic111  fic112"
echo -e "\x1b[0m"
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120mCreamos más directorios de los que caben en 1 bloque\x1b[0m"
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_mkdir disco /dir3/\x1b[0m"
./mi_mkdir disco 6 /dir3/
echo -e "\x1B[38;2;17;245;120mcreamos 17 subdirectorios sd0, sd1..., sd16 en /dir3/"
echo "$ for i in \$(seq 0 16)"
echo "> do"
echo "> ./mi_mkdir disco 6 /dir3/sd\$i/"
echo "> done"
echo -e "\x1b[0m"
for i in $(seq 0 16)
do
    ./mi_mkdir disco 6 /dir3/sd$i/
done
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m"
echo "             /"
echo "        _____|_____" 
echo "       /     |     \\"
echo "     dir1  dir2    dir3"
echo "      |        ______|_____________________________________________________________"
echo "      |       /  |   |   |   |   |   |   |   |   |   |    |    |    |    |    |    \\"
echo "    dir11   sd0 sd1 sd2 sd3 sd4 sd5 sd6 sd7 sd8 sd9 sd10 sd11 sd12 sd13 sd14 sd15 sd16"
echo "   /     \\"
echo "fic111  fic112"
echo -e "\x1b[0m"
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120mMostramos la metainformacion del directorio para ver que tiene 2 bloques de datos\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_stat disco /dir3/\x1b[0m"
./mi_stat disco /dir3/
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120mListamos el directorio para ver sus subdirectorios\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_ls disco /dir3/\x1b[0m"
./mi_ls disco /dir3/
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120mEliminamos el subdirectorio sd3 de dir3\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_rmdir disco /dir3/sd3/\x1b[0m"
./mi_rmdir disco /dir3/sd3/
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m"
echo "             /"
echo "        _____|_____" 
echo "       /     |     \\"
echo "     dir1  dir2    dir3"
echo "      |         ____|__________________________________________________________"
echo "      |        /  |   |   |   |   |   |   |   |   |   |    |    |    |    |    \\"
echo "    dir11   sd0 sd1 sd2 sd16 sd4 sd5 sd6 sd7 sd8 sd9 sd10 sd11 sd12 sd13 sd14 sd15"
echo "   /     \\"
echo "fic111  fic112"
echo -e "\x1b[0m"
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120mMostramos la metainformacion de dir3 para ver que ahora tiene 1 bloque de datos\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_stat disco /dir3/\x1b[0m"
./mi_stat disco /dir3/
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120mVolvemos a listar el directorio para ver que se ha eliminado un subdirectorio\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_ls disco /dir3/\x1b[0m"
./mi_ls disco /dir3/

make clean