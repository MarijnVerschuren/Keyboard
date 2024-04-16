rm ./soft/doc -rf
rm ./soft/include -rf
rm ./soft/src/*.c
rm ./soft/src/usb -rf

cp ./STM32H7B0_CMSIS/doc ./soft/doc -r
cp ./STM32H7B0_CMSIS/include ./soft/include -r
cp ./STM32H7B0_CMSIS/src/* ./soft/src/ -r
rm ./soft/src/main.c
