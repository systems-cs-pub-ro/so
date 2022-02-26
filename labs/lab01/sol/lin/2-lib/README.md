# 02-lib

Pentru a putea folosi biblioteca partajata trebuie exportata variabila `LD_LIBRARY_PATH`

Pentru a determina ce biblioteci partajate foloseste un executabil, se poate folosi comanda `ldd`:
```
student@vagrant:~/so/labs/lab01/skel/lin/2-lib$ make -f Makefile_dynamic
gcc -Wall -fPIC   -c -o main.o main.c
gcc -Wall -fPIC   -c -o hexdump.o hexdump.c
gcc -Wall -fPIC   -c -o sample.o sample.c
gcc -shared -o libhexdump_dynamic.so hexdump.o sample.o
gcc -Wall -fPIC -o main_dynamic main.o -lhexdump_dynamic -L.

student@vagrant:~/so/labs/lab01/skel/lin/2-lib$ ls
Makefile_dynamic  Makefile_static  README.md  hexdump.c  hexdump.h  hexdump.o  libhexdump_dynamic.so  main.c  main.o  main_dynamic  sample.c  sample.o

student@vagrant:~/so/labs/lab01/skel/lin/2-lib$ ./main_dynamic
./main_dynamic: error while loading shared libraries: libhexdump_dynamic.so: cannot open shared object file: No such file or directory

student@vagrant:~/so/labs/lab01/skel/lin/2-lib$ LD_LIBRARY_PATH=. ./main_dynamic
08 7b ae 70 f5 b0 0b 3e 4d 51 c3 4c f5 2b d8 bc  .{.p...>MQ.L.
```
