#!/bin/bash

./hpc-clust -al true -ofile tests/single-al examples/aligned-archaea-seqs.sto &> /dev/null

if [ ! -f "tests/single-sl.sl" ]; then exit 99; fi # output file not found

OTUCOUNT=`tail -n 1 tests/single-al.al.ref | cut -d ' ' -f 1`
LASTDIST=`tail -n 1 tests/single-al.al.ref | cut -d ' ' -f 2`

OTUCOUNT2=`tail -n 1 tests/single-al.al | cut -d ' ' -f 1`
LASTDIST2=`tail -n 1 tests/single-al.al | cut -d ' ' -f 2`

#if [ "$OTUCOUNT" != "$OTUCOUNT2" -o "$LASTDIST" != "$LASTDIST2" ]; then exit 99; fi

./make-otus.sh examples/aligned-archaea-seqs.sto tests/single-al.al 0.98 > tests/single-al.al.0.98.otu

./make-otus.sh examples/aligned-archaea-seqs.fasta tests/single-al.al.ref 0.98 > tests/single-al.fasta.al.ref.0.98.otu

exit 0
