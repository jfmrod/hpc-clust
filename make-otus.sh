#!/bin/bash

if [ -z "$3" ]; then
  echo "syntax: $( basename $0 ) <alignedseqs.sto> <mergefile.sl> <threshold>";
  exit -1;
fi

awk -v "joinfile=$2" -v "seqfile=$1" -v "thres=$3" '
BEGIN {
  seqcount=0;
  while (getline<seqfile > 0){
    if (/^(#|$)/) continue;
    seqs[seqcount++]=$1;
  }
}

/^(#|$)/ { next; }

$2 >= thres {
  seqs[$3]=seqs[$3] SUBSEP seqs[$4];
  delete seqs[$4];
  next;
}

END {
  print "# date: " strftime();
  print "# merge file: " joinfile;
  print "# sequences file: " seqfile;
  print "# threshold: " thres;
  print "# seqcount: " seqcount;
  otucount=0;
  for (i=0; i<seqcount; ++i) {
    if (i in seqs) {
      split(seqs[i],a,SUBSEP);
      otusize=length(a);
      print ">OTU" otucount "\tsize=" otusize;
      for (j=1; j<=length(a); ++j)
        print a[j];
      ++otucount;
    }
  }
}' $2 
