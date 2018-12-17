#!/bin/tcsh





ls ../c01_tools/hello2.c

gcc -c ../c01_tools/*.c  -I/opt/sac/include -L/opt/sac/lib

ar cr libHongyu.a *.o


gcc    ./empirical_wavelet.c    -L. -lHongyu -L/opt/sac/lib -lsacio -I/opt/sac/include


./a.out
