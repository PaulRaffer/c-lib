readonly RAFFER_C_ROOT=".."
readonly RAFFER_C_INCLUDE="$RAFFER_C_ROOT/include"

mkdir build
cd build

gcc -c $RAFFER_C_INCLUDE/raffer/error.c
ar rcs liberror.a error.o

gcc -c $RAFFER_C_INCLUDE/raffer/str.c
ar rcs libstr.a str.o

gcc -c $RAFFER_C_INCLUDE/raffer/file.c
ar rcs libfile.a file.o

gcc -c $RAFFER_C_INCLUDE/raffer/process.c
ar rcs libprocess.a process.o

ar rcs libraffer.a error.o str.o file.o process.o
