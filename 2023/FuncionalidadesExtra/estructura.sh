make clean
make
echo "ESTE SCRIPT CREA VARIOS DIRECTORIOS Y FICHEROS, CON INFORMACIÓN EN VARIOS OFFSETS CON EL PROPÓSITO DE HACER PRUEBAS"
echo "######################################################################"
echo "$ ./mi_mkfs disco 100000"
./mi_mkfs disco 100000
echo "######################################################################"
echo "Creamos un set de directorios y ficheros para hacer pruebas"
echo "$ ./mi_mkdir disco 6 /dir1/"
./mi_mkdir disco 6 /dir1/
echo "$ ./mi_mkdir disco 6 /dir1/dir11/"
./mi_mkdir disco 6 /dir1/dir11/
echo "$ ./mi_mkdir disco 6 /dir1/dir12/"
./mi_mkdir disco 6 /dir1/dir12/
echo "$ ./mi_touch disco 6 /dir1/dir12/fic121/"
./mi_touch disco 6 /dir1/dir12/fic121
echo "$ ./mi_touch disco 6 /dir1/dir12/fic122/"
./mi_touch disco 6 /dir1/dir12/fic122
echo "$ ./mi_mkdir disco 6 /dir1/dir13/"
./mi_mkdir disco 6 /dir1/dir13/
echo "$ ./mi_touch disco 6 /dir1/fic11/"
./mi_touch disco 6 /dir1/fic11
echo "$ ./mi_mkdir disco 6 /dir2/"
./mi_mkdir disco 6 /dir2/
echo "$ ./mi_mkdir disco 6 /dir2/dir21/"
./mi_mkdir disco 6 /dir2/dir21/
echo "$ ./mi_touch disco 6 /dir2/dir21/fic211"
./mi_touch disco 6 /dir2/dir21/fic211
echo "$ ./mi_mkdir disco 6 /dir2/dir22/"
./mi_mkdir disco 6 /dir2/dir22/
echo "$ ./mi_touch disco 6 /dir2/dir22/fic221"
./mi_touch disco 6 /dir2/dir22/fic221
echo "$ ./mi_mkdir disco 6 /dir2/dir23/"
./mi_mkdir disco 6 /dir2/dir23/
echo "$ ./mi_mkdir disco 6 /dir3/"
./mi_mkdir disco 6 /dir3/
echo "$ ./mi_touch disco 6 /dir3/fic31"
./mi_touch disco 6 /dir3/fic31
echo "$ ./mi_mkdir disco 6 /dir3/dir31/"
./mi_mkdir disco 6 /dir3/dir31/
echo "$ ./mi_mkdir disco 6 /dir3/dir32/"
./mi_mkdir disco 6 /dir3/dir32/
echo "$ ./mi_mkdir disco 6 /dir3/dir33/"
./mi_mkdir disco 6 /dir3/dir33/
echo "######################################################################"
echo "Metemos información en diferentes offsets de los ficheros"
echo "$ ./mi_escribir disco /dir1/dir12/fic121 \"Haz un buen día - \" 0"
./mi_escribir disco /dir1/dir12/fic121 "Haz un buen día - " 0
echo "$ ./mi_escribir disco /dir1/dir12/fic121 \"Hoy puede ser un gran día, plantéatelo así. Aprovecharlo o que pase de largo depende en parte de ti. - \" 5120 "
./mi_escribir disco /dir1/dir12/fic121 "Hoy puede ser un gran día, plantéatelo así. Aprovecharlo o que pase de largo depende en parte de ti. - " 5120
echo "$ ./mi_escribir disco /dir1/dir12/fic121 \"Si la rutina te aplasta dile que ya basta de mediocridad - \" 48000 "
./mi_escribir disco /dir1/dir12/fic121 "Si la rutina te aplasta dile que ya basta de mediocridad - " 48000
echo "$ ./mi_escribir disco /dir1/dir12/fic122 \"Pelea por lo que quieres y no desesperes si algo no anda bien - \" 90000"
./mi_escribir disco /dir1/dir12/fic122 "Pelea por lo que quieres y no desesperes si algo no anda bien - " 90000
echo "$ ./mi_escribir disco /dir1/dir12/fic122 \"Hoy puede ser un gran día y mañana también - \" 15000"
./mi_escribir disco /dir1/dir12/fic122 "Hoy puede ser un gran día y mañana también - " 15000
echo "$ ./mi_escribir disco /dir1/fic11 \"Hoy puede ser un gran día, date una oportunidad - \" 70000"
./mi_escribir disco /dir1/fic11 "Hoy puede ser un gran día, date una oportunidad - " 70000
echo "$ ./mi_escribir disco /dir2/dir21/fic211 \"Hoy puede ser un gran día, imposible de recuperar - \" 100"
./mi_escribir disco /dir2/dir21/fic211 "Hoy puede ser un gran día, imposible de recuperar - " 100
echo "$ ./mi_escribir disco /dir2/dir22/fic221 \"Hoy puede ser un gran día, duro con él - \" 3333"
./mi_escribir disco /dir2/dir22/fic221 "Hoy puede ser un gran día, duro con él - " 3333
echo "$ ./mi_escribir disco /dir3/fic31 \"Que la fuerza te acompañe - \" 1000"
./mi_escribir disco /dir3/fic31 "Que la fuerza te acompañe - " 1000
echo "######################################################################"
echo "Comprobamos el contenido de algunos directorios"
echo "$ ./mi_ls disco /"
./mi_ls disco /
echo "$ ./mi_ls disco /dir1/"
./mi_ls disco /dir1/
echo "$ ./mi_cat disco /dir1/fic11"
./mi_cat disco /dir1/fic11
echo "$ ./mi_stat disco /dir1/fic11"
./mi_stat disco /dir1/fic11
echo "$ ./mi_ls disco /dir1/dir12/"
./mi_ls disco /dir1/dir12/
echo "$ ./mi_cat disco /dir1/dir12/fic121"
./mi_cat disco /dir1/dir12/fic121
echo "$ ./mi_stat disco /dir1/dir12/fic121"
./mi_stat disco /dir1/dir12/fic121
echo "$ ./mi_cat disco /dir1/dir12/fic122"
./mi_cat disco /dir1/dir12/fic122
echo "$ ./mi_stat disco /dir1/dir12/fic122"
./mi_stat disco /dir1/dir12/fic122
echo "$ ./mi_ls disco /dir2/"
./mi_ls disco /dir2/
echo "$ ./mi_ls disco /dir2/dir21/"
./mi_ls disco /dir2/dir21/
echo "$ ./mi_cat disco /dir2/dir21/fic211"
./mi_cat disco /dir2/dir21/fic211
echo "$ ./mi_stat disco /dir2/dir21/fic211"
./mi_stat disco /dir2/dir21/fic211
echo "$ ./mi_ls disco /dir2/dir22/"
./mi_ls disco /dir2/dir22/
echo "$ ./mi_cat disco /dir2/dir22/fic221"
./mi_cat disco /dir2/dir22/fic221
echo "$ ./mi_stat disco /dir2/dir22/fic221"
./mi_stat disco /dir2/dir22/fic221
echo "$ ./mi_ls disco /dir3/"
./mi_ls disco /dir3/
echo "$ ./mi_cat disco /dir3/fic31"
./mi_cat disco /dir3/fic31
echo "$ ./mi_stat disco /dir3/fic31"
./mi_stat disco /dir3/fic31
echo "$ ./leer_sf disco"
./leer_sf disco