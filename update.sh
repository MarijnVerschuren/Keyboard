git submodule foreach git pull origin main

mv ./software/src/main.c main.c

rm ./software/inc -rf
rm ./software/lib -rf
rm ./software/lnk -rf
rm ./software/src -rf
rm ./software/targets -rf
rm ./software/cmake -rf
rm ./software/.run -rf
rm ./software/CMakeLists.txt
rm ./software/.gitignore

cp ./STM32F412/inc ./software/ -r
cp ./STM32F412/lib ./software/ -r
cp ./STM32F412/lnk ./software/ -r
cp ./STM32F412/src ./software/ -r
cp ./STM32F412/targets ./software/ -r
cp ./STM32F412/cmake ./software/ -r
cp ./STM32F412/.run ./software/ -r
cp ./STM32F412/CMakeLists.txt ./software/
cp ./STM32F412/.gitignore ./software/

rm ./software/src/main.c
mv main.c ./software/src

