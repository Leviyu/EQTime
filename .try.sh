#!/bin/tcsh


set CODE = /mnt/data2/maligaro/EQTime/Maligaro/c_lib/lib
cd $CODE
make 


set WORK = /mnt/data2/maligaro/EQTime/DATADIR/T11.1/201907060319

cd $WORK
cp $CODE/EWM .

pwd
./EWM $WORK/input.c

