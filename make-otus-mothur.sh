#!/bin/bash

if [ -z "$3" ]; then
  echo "syntax: $( basename $0 ) <alignedseqs.sto> <mergefile.sl> <threshold>";
  exit -1;
fi

awk -v "joinfile=$2" -v "seqfile=$1" -v "thres=$3" '
BEGIN {
  ci=0;
  for (i=0; i<=10; ++i)
    cthres[i]=1.0-0.01*i;
    
  seqcount=0;
  fasta=0;
  while (getline<seqfile > 0){
    if (/^(#|$)/) continue;
    if (substr($0,1,1)==">") fasta=1;
    if (fasta==1 && /^>/){
      $1=substr($1,2);
      seqs[seqcount++]=$1;
    }else if (fasta==0)
      seqs[seqcount++]=$1;
  }
  lthres=1.0;
  maxotus=0;
}

/^(#|$)/ { next; }

lthres != $2 && $2 < cthres[ci] {
  if (maxotus==0) maxotus=length(seqs);
  str=(1.0-cthres[ci]) "\t" length(seqs);
  for (i=0; i<seqcount; ++i) {
    if (i in seqs) {
      split(seqs[i],a,SUBSEP);
      otustr="";
      for (j=1; j<=length(a); ++j)
        otustr=otustr "," a[j];
      str=str "\t" substr(otustr,2);
    }
  }
  otus[ci]=str;
  ++ci;
}

ci in cthres {
  seqs[$3]=seqs[$3] SUBSEP seqs[$4];
  delete seqs[$4];
  lthres=$2;
  next;
}

END {
  str=(1.0-lthres) "\t" length(seqs);
  for (i=0; i<seqcount; ++i) {
    if (i in seqs) {
      split(seqs[i],a,SUBSEP);
      otustr="";
      for (j=1; j<=length(a); ++j)
        otustr=otustr "," a[j];
      str=str "\t" substr(otustr,2);
    }
  }
  otus[ci]=str;

  OFS="\t";
  str="";
  for (i=0; i<maxotus; ++i)
    str=str "\t" "Otu" (i+1);
  print "label","numOtus" str;
  for (i=0; i in otus; ++i)
  print otus[i];
}' $2 
