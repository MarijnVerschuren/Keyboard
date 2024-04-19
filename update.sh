git submodule foreach git pull origin main

rm ./soft/doc -rf
rm ./soft/include/*.h
rm ./soft/src/*.c
rm ./soft/include/usb -rf
rm ./soft/include/spi -rf
rm ./soft/src/usb -rf
rm ./soft/test -rf

cp ./STM32H7B0_CMSIS/doc ./soft/doc -r
cp ./STM32H7B0_CMSIS/include/* ./soft/include/ -r
cp ./STM32H7B0_CMSIS/src/* ./soft/src/ -r
rm ./soft/src/main.c
cp ./STM32H7B0_CMSIS/test ./soft/test -r

