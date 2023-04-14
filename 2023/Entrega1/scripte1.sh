# Marc Llobera Villalonga
# scripte1.sh

clear
make clean
make
echo "################################################################################"
echo -e "\x1B[38;2;17;245;120m$ ./mi_mkfs disco 100000\x1b[0m"
./mi_mkfs disco 100000
echo
echo -e "\x1B[38;2;17;245;120m$ ./leer_sf disco\x1b[0m"
./leer_sf disco
echo
echo "################################################################################"
echo -e "\x1B[38;2;17;245;120m$./escribir\x1b[0m"
./escribir
echo
echo "################################################################################"
echo -e "\x1B[38;2;17;245;120m$ ./escribir disco "$(cat texto2.txt)" 0"
echo -e "#escribimos el texto2.txt en los offsets  9.000, 209.000, 30.725.000,"
echo -e "# 409.605.000 y 480.000.000 de un mismo inodo\x1b[0m"
./escribir disco "$(cat texto2.txt)" 0
echo
echo "################################################################################"
echo -e "\x1B[38;2;17;245;120m$ ./permitir disco 1 0"
echo -e "#cambiamos permisos del inodo 1 a 0\x1b[0m"
./permitir disco 1 0
echo
echo -e "\x1B[38;2;17;245;120m$ ./leer disco 1"
echo -e "#intentamos leer inodo 1 con permisos=0\x1b[0m"
./leer disco 1
echo
echo "################################################################################"
echo -e "\x1B[38;2;17;245;120m$ ./permitir disco 1 6"
echo -e "#cambiamos permisos del inodo 1 a 6\x1b[0m"
./permitir disco 1 6
echo
echo -e "\x1B[38;2;17;245;120m$ ./leer disco 1 > ext1.txt\x1b[0m"
./leer disco 1 > ext1.txt
echo
echo -e "\x1B[38;2;17;245;120m$ ls -l ext1.txt\x1b[0m"
ls -l ext1.txt
echo
echo -e "\x1B[38;2;17;245;120m$ ./leer_sf disco\x1b[0m"
./leer_sf disco
echo
echo "################################################################################"
echo -e "\x1B[38;2;17;245;120m$ ./truncar disco 1 409605001\x1b[0m"
./truncar disco 1 409605001
echo
echo -e "\x1B[38;2;17;245;120m$ ./leer_sf disco\x1b[0m"
./leer_sf disco
echo
echo "################################################################################"
echo -e "\x1B[38;2;17;245;120m$ ./truncar disco 1 30725003\x1b[0m"
./truncar disco 1 30725003
echo
echo -e "\x1B[38;2;17;245;120m$ ./leer_sf disco\x1b[0m"
./leer_sf disco
echo
echo "################################################################################"
echo -e "\x1B[38;2;17;245;120m$ ./truncar disco 1 209008\x1b[0m"
./truncar disco 1 209008
echo
echo -e "\x1B[38;2;17;245;120m$ ./leer_sf disco\x1b[0m"
./leer_sf disco
echo
echo "################################################################################"
echo -e "\x1B[38;2;17;245;120m$ ./truncar disco 1 9005\x1b[0m"
./truncar disco 1 9005
echo 
echo -e "\x1B[38;2;17;245;120m$ ./leer disco 1 > ext1.txt\x1b[0m"
./leer disco 1 > ext1.txt
echo
echo -e "\x1B[38;2;17;245;120m$ ls -l ext1.txt\x1b[0m"
ls -l ext1.txt
echo
echo -e "\x1B[38;2;17;245;120m$ cat ext1.txt\x1b[0m"
cat ext1.txt
echo
echo
echo -e "\x1B[38;2;17;245;120m$ ./leer_sf disco\x1b[0m"
./leer_sf disco
echo "################################################################################"
echo -e "\x1B[38;2;17;245;120m$ ./truncar disco 1 0\x1b[0m"
./truncar disco 1 0
echo
echo -e "\x1B[38;2;17;245;120m$ ./leer_sf disco\x1b[0m"
./leer_sf disco
echo "################################################################################"

make clean
