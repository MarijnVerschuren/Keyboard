cd STM32H7B0_CMSIS
git submodule update --init
cd ..

rm ./soft/doc -rf
rm ./soft/include -rf
rm ./soft/src/*.c
rm ./soft/src/usb -rf
rm ./soft/test -rf

cp ./STM32H7B0_CMSIS/doc ./soft/doc -r
cp ./STM32H7B0_CMSIS/include ./soft/include -r
cp ./STM32H7B0_CMSIS/src/* ./soft/src/ -r
rm ./soft/src/main.c
cp ./STM32H7B0_CMSIS/test ./soft/test -r

