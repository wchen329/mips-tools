# UNIX Build script for MIPS-Tools
cp -r src/*.cpp build
cp -r include/*.h build
cd build
g++ -c `ls *.cpp`
g++ `ls *.o` -o mtshell
