#!/bin/bash

./hpc-clust -sl true -ofile tests/single-sl examples/aligned-archaea-seqs.sto &> /dev/null

if [ ! -f "tests/single-sl.sl" ]; then exit 99; fi # output file not found

OTUCOUNT=`tail -n 1 tests/single-sl.sl.ref | cut -d ' ' -f 1`
LASTDIST=`tail -n 1 tests/single-sl.sl.ref | cut -d ' ' -f 2`

OTUCOUNT2=`tail -n 1 tests/single-sl.sl | cut -d ' ' -f 1`
LASTDIST2=`tail -n 1 tests/single-sl.sl | cut -d ' ' -f 2`

if [ "$OTUCOUNT" != "$OTUCOUNT2" -o "$LASTDIST" != "$LASTDIST2" ]; then exit 99; fi

exit 0
