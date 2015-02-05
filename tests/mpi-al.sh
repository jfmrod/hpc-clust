#!/bin/bash

mpirun -n 10 ./hpc-clust-mpi -al true -ofile tests/mpi-al examples/aligned-archaea-seqs.sto &> /dev/null

if [ ! -f "tests/mpi-al.al" ]; then exit 99; fi # output file not found

OTUCOUNT=`tail -n 1 tests/single-al.al.ref | cut -d ' ' -f 1`
LASTDIST=`tail -n 1 tests/single-al.al.ref | cut -d ' ' -f 2`

OTUCOUNT2=`tail -n 1 tests/mpi-al.al | cut -d ' ' -f 1`
LASTDIST2=`tail -n 1 tests/mpi-al.al | cut -d ' ' -f 2`

if [ "$OTUCOUNT" != "$OTUCOUNT2" -o "$LASTDIST" != "$LASTDIST2" ]; then exit 99; fi

exit 0
