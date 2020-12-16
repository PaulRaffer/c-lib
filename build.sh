mkdir build
cd build

gcc -c ../include/error.c
ar rcs liberror.a error.o

gcc -c ../include/str.c
ar rcs libstr.a str.o

gcc -c ../include/file.c
ar rcs libfile.a file.o

gcc -c ../include/process.c
ar rcs libprocess.a process.o

ar rcs libraffer.a error.o str.o file.o process.o
