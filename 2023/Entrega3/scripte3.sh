make clean
make

#Creamos dispositivo
./mi_mkfs disco 100000

#Obtenemos la fecha
current_date=`date +%Y%m%d%H%M%S` #yearmonthdayHourMinuteSecond
#Lanzamos simulación
echo
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ time ./simulacion disco\x1b[0m"
time ./simulacion disco 

#Modificamos el string del directorio para la verificación
sim_dir="simul_"$current_date #simul_aaaammddhhmmss

#Lanzamos verificación
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ time ./verificacion disco /$sim_dir/\x1b[0m"
time ./verificacion disco /$sim_dir/

#Redireccionamos el informe a res.txt
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./mi_cat disco /$sim_dir/informe.txt > res.txt\x1b[0m"
./mi_cat disco /$sim_dir/informe.txt > res.txt

#Mostramos datos de res.txt
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ls -l res.txt\x1b[0m"
ls -l res.txt

#Mostramos el informe
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ cat res.txt\x1b[0m"
cat res.txt

#Mostramos el SB
echo -e "\x1B[38;2;17;245;120m######################################################################\x1b[0m"
echo -e "\x1B[38;2;17;245;120m$ ./leer_sf disco\x1b[0m"
./leer_sf disco

make clean