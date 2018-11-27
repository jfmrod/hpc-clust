#!/bin/bash

./hpc-clust -al true -ofile tests/single-al examples/aligned-archaea-seqs.sto &> /dev/null

if [ ! -f "tests/single-al.al" ]; then
  echo "output file not found";
  exit 99;
fi # output file not found

OTUCOUNT=`tail -n 1 tests/single-al.al.ref | cut -d ' ' -f 1`
LASTDIST=`tail -n 1 tests/single-al.al.ref | cut -d ' ' -f 2`

OTUCOUNT2=`tail -n 1 tests/single-al.al | cut -d ' ' -f 1`
LASTDIST2=`tail -n 1 tests/single-al.al | cut -d ' ' -f 2`

awk 'BEGIN { oc='$OTUCOUNT'; oc2='$OTUCOUNT2'; ld='$LASTDIST'; ld2='$LASTDIST2'; doc=oc2-oc; dld=ld2-ld; doc=(doc<0?-doc:doc); dld=(dld<0.0?-dld:dld); if (doc>=2 || dld>=0.01*ld) exit(99); exit(0); }'
if [ "$?" = "99" ]; then
  echo "wrong number of otucounts: $OTUCOUNT != $OTUCOUNT2 or last dist: $LASTDIST != $LASTDIST2"
  exit 99;
fi

./make-otus.sh examples/aligned-archaea-seqs.sto tests/single-al.al 0.98 > tests/single-al.al.0.98.otu

./make-otus.sh examples/aligned-archaea-seqs.fasta tests/single-al.al.ref 0.98 > tests/single-al.fasta.al.ref.0.98.otu

exit 0
