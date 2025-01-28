gcc -c utils/inout.c
gcc -c utils/list.c
gcc -c utils/array.c
@REM gcc -c utils/imageImporter.c -D SL_DONT_USE_PNG
gcc -c utils/imageImporter.c
gcc -c utils/arenaAlloc.c
gcc -c utils/hashtbl.c
gcc -c utils/argument.c
@REM gcc -c utils/puff.c -D SL_DONT_USE_PNG
gcc -c utils/puff.c

gcc -c maths/math.c
gcc -c maths/vector.c
gcc -c maths/quaternion.c
gcc -c maths/matrix.c

ar rc libSL.a **.o
ranlib libSL.a

copy libSL.a C:\msys64\mingw64\lib\libSL.a
del libSL.a
del **.o

nm C:\msys64\mingw64\lib\libSL.a
pause