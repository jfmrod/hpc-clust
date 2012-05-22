#include <eutils/emain.h>
#include <eutils/efile.h>
#include <eutils/eheap.h>
#include <eutils/etime.h>
#include <eutils/etimer.h>
#include <eutils/eoption.h>
#include <eutils/ethread.h>

#include "cluster-common.h"

estrarray arr;
estrarray arrshort;
estrhashof<int> seqotu;
eintarray otucount;
eintarray oturef;

int seqlen=0;

estr args2str(int argvc,char **argv)
{
  estr tmpstr;
  int i;
  for (i=0; i<argvc; ++i)
    tmpstr+=estr(argv[i])+" ";
  tmpstr.del(-1);
  return(tmpstr);
}

inline float dist_tamura_short_compressed(const estr& s1,const estr& s2,int s,int seqlen)
{
  int P=0; // transitions
  int Q=0; // transversions
  int GC1=0; // GC content seq1
  int GC2=0; // GC content seq2

  int len=seqlen-s;
  int count=0;
  long int *ep1=(long int*)(s1._str)+MIN(s1._strlen/8,seqlen/16);
  long int *p1=(long int*)(s1._str)+(s/16);
  long int *p2=(long int*)(s2._str)+(s/16);

  if (p1<ep1){
    switch (s%16){
     case 1:
      dist_tamura_inc(*p1,*p2,b4_m1,P,Q,GC1,GC2,len);
     case 2:
      dist_tamura_inc(*p1,*p2,b4_m2,P,Q,GC1,GC2,len);
     case 3:
      dist_tamura_inc(*p1,*p2,b4_m3,P,Q,GC1,GC2,len);
     case 4:
      dist_tamura_inc(*p1,*p2,b4_m4,P,Q,GC1,GC2,len);
     case 5:
      dist_tamura_inc(*p1,*p2,b4_m5,P,Q,GC1,GC2,len);
     case 6:
      dist_tamura_inc(*p1,*p2,b4_m6,P,Q,GC1,GC2,len);
     case 7:
      dist_tamura_inc(*p1,*p2,b4_m7,P,Q,GC1,GC2,len);
     case 8:
      dist_tamura_inc(*p1,*p2,b4_m8,P,Q,GC1,GC2,len);
     case 9:
      dist_tamura_inc(*p1,*p2,b4_m9,P,Q,GC1,GC2,len);
     case 10:
      dist_tamura_inc(*p1,*p2,b4_m10,P,Q,GC1,GC2,len);
     case 11:
      dist_tamura_inc(*p1,*p2,b4_m11,P,Q,GC1,GC2,len);
     case 12:
      dist_tamura_inc(*p1,*p2,b4_m12,P,Q,GC1,GC2,len);
     case 13:
      dist_tamura_inc(*p1,*p2,b4_m13,P,Q,GC1,GC2,len);
     case 14:
      dist_tamura_inc(*p1,*p2,b4_m14,P,Q,GC1,GC2,len);
     case 15:
      dist_tamura_inc(*p1,*p2,b4_m15,P,Q,GC1,GC2,len);
      ++p1;
      ++p2;
    }
  }

  for (; p1<ep1; ++p1,++p2){
    dist_tamura_inc(*p1,*p2,b4_m0,P,Q,GC1,GC2,len);
    dist_tamura_inc(*p1,*p2,b4_m1,P,Q,GC1,GC2,len);
    dist_tamura_inc(*p1,*p2,b4_m2,P,Q,GC1,GC2,len);
    dist_tamura_inc(*p1,*p2,b4_m3,P,Q,GC1,GC2,len);
    dist_tamura_inc(*p1,*p2,b4_m4,P,Q,GC1,GC2,len);
    dist_tamura_inc(*p1,*p2,b4_m5,P,Q,GC1,GC2,len);
    dist_tamura_inc(*p1,*p2,b4_m6,P,Q,GC1,GC2,len);
    dist_tamura_inc(*p1,*p2,b4_m7,P,Q,GC1,GC2,len);
    dist_tamura_inc(*p1,*p2,b4_m8,P,Q,GC1,GC2,len);
    dist_tamura_inc(*p1,*p2,b4_m9,P,Q,GC1,GC2,len);
    dist_tamura_inc(*p1,*p2,b4_m10,P,Q,GC1,GC2,len);
    dist_tamura_inc(*p1,*p2,b4_m11,P,Q,GC1,GC2,len);
    dist_tamura_inc(*p1,*p2,b4_m12,P,Q,GC1,GC2,len);
    dist_tamura_inc(*p1,*p2,b4_m13,P,Q,GC1,GC2,len);
    dist_tamura_inc(*p1,*p2,b4_m14,P,Q,GC1,GC2,len);
    dist_tamura_inc(*p1,*p2,b4_m15,P,Q,GC1,GC2,len);
  }

  switch (seqlen%16){
   case 15:
    dist_tamura_inc(*p1,*p2,b4_m14,P,Q,GC1,GC2,len);
   case 14:
    dist_tamura_inc(*p1,*p2,b4_m13,P,Q,GC1,GC2,len);
   case 13:
    dist_tamura_inc(*p1,*p2,b4_m12,P,Q,GC1,GC2,len);
   case 12:
    dist_tamura_inc(*p1,*p2,b4_m11,P,Q,GC1,GC2,len);
   case 11:
    dist_tamura_inc(*p1,*p2,b4_m10,P,Q,GC1,GC2,len);
   case 10:
    dist_tamura_inc(*p1,*p2,b4_m9,P,Q,GC1,GC2,len);
   case 9:
    dist_tamura_inc(*p1,*p2,b4_m8,P,Q,GC1,GC2,len);
   case 8:
    dist_tamura_inc(*p1,*p2,b4_m7,P,Q,GC1,GC2,len);
   case 7:
    dist_tamura_inc(*p1,*p2,b4_m6,P,Q,GC1,GC2,len);
   case 6:
    dist_tamura_inc(*p1,*p2,b4_m5,P,Q,GC1,GC2,len);
   case 5:
    dist_tamura_inc(*p1,*p2,b4_m4,P,Q,GC1,GC2,len);
   case 4:
    dist_tamura_inc(*p1,*p2,b4_m3,P,Q,GC1,GC2,len);
   case 3:
    dist_tamura_inc(*p1,*p2,b4_m2,P,Q,GC1,GC2,len);
   case 2:
    dist_tamura_inc(*p1,*p2,b4_m1,P,Q,GC1,GC2,len);
   case 1:
    dist_tamura_inc(*p1,*p2,b4_m0,P,Q,GC1,GC2,len);
  }

  float C=GC1/seqlen + GC2/seqlen - 2.0*GC1/seqlen*GC2/seqlen;
  return(1+1.0*C*log(1.0-(float)P/(C*len)-(float)Q/len) + 0.5*(1.0-C)*log(1.0-2.0*Q/len));
}



inline float dist_short_compressed(const estr& s1,const estr& s2,int s,int seqlen)
{
  int len=seqlen-s;
  int count=0;
  long int *ep1=(long int*)(s1._str)+MIN(s1._strlen/8,seqlen/16);
  long int *p1=(long int*)(s1._str)+(s/16);
  long int *p2=(long int*)(s2._str)+(s/16);

  if (p1<ep1){
    switch (s%16){
     case 1:
      dist_inc(*p1,*p2,b4_m1,count);
     case 2:
      dist_inc(*p1,*p2,b4_m2,count);
     case 3:
      dist_inc(*p1,*p2,b4_m3,count);
     case 4:
      dist_inc(*p1,*p2,b4_m4,count);
     case 5:
      dist_inc(*p1,*p2,b4_m5,count);
     case 6:
      dist_inc(*p1,*p2,b4_m6,count);
     case 7:
      dist_inc(*p1,*p2,b4_m7,count);
     case 8:
      dist_inc(*p1,*p2,b4_m8,count);
     case 9:
      dist_inc(*p1,*p2,b4_m9,count);
     case 10:
      dist_inc(*p1,*p2,b4_m10,count);
     case 11:
      dist_inc(*p1,*p2,b4_m11,count);
     case 12:
      dist_inc(*p1,*p2,b4_m12,count);
     case 13:
      dist_inc(*p1,*p2,b4_m13,count);
     case 14:
      dist_inc(*p1,*p2,b4_m14,count);
     case 15:
      dist_inc(*p1,*p2,b4_m15,count);
      ++p1;
      ++p2;
    }
  }

  for (; p1<ep1; ++p1,++p2){
    dist_inc(*p1,*p2,b4_m0,count);
    dist_inc(*p1,*p2,b4_m1,count);
    dist_inc(*p1,*p2,b4_m2,count);
    dist_inc(*p1,*p2,b4_m3,count);
    dist_inc(*p1,*p2,b4_m4,count);
    dist_inc(*p1,*p2,b4_m5,count);
    dist_inc(*p1,*p2,b4_m6,count);
    dist_inc(*p1,*p2,b4_m7,count);
    dist_inc(*p1,*p2,b4_m8,count);
    dist_inc(*p1,*p2,b4_m9,count);
    dist_inc(*p1,*p2,b4_m10,count);
    dist_inc(*p1,*p2,b4_m11,count);
    dist_inc(*p1,*p2,b4_m12,count);
    dist_inc(*p1,*p2,b4_m13,count);
    dist_inc(*p1,*p2,b4_m14,count);
    dist_inc(*p1,*p2,b4_m15,count);
  }

  switch (seqlen%16){
   case 15:
    dist_inc(*p1,*p2,b4_m14,count);
   case 14:
    dist_inc(*p1,*p2,b4_m13,count);
   case 13:
    dist_inc(*p1,*p2,b4_m12,count);
   case 12:
    dist_inc(*p1,*p2,b4_m11,count);
   case 11:
    dist_inc(*p1,*p2,b4_m10,count);
   case 10:
    dist_inc(*p1,*p2,b4_m9,count);
   case 9:
    dist_inc(*p1,*p2,b4_m8,count);
   case 8:
    dist_inc(*p1,*p2,b4_m7,count);
   case 7:
    dist_inc(*p1,*p2,b4_m6,count);
   case 6:
    dist_inc(*p1,*p2,b4_m5,count);
   case 5:
    dist_inc(*p1,*p2,b4_m4,count);
   case 4:
    dist_inc(*p1,*p2,b4_m3,count);
   case 3:
    dist_inc(*p1,*p2,b4_m2,count);
   case 2:
    dist_inc(*p1,*p2,b4_m1,count);
   case 1:
    dist_inc(*p1,*p2,b4_m0,count);
  }
  return((float)count/(float)(seqlen-s));
}

inline float dist_nogap_short_compressed(const estr& s1,const estr& s2,int s,int seqlen)
{
  int len=seqlen-s;
  int count=0;
  long int *ep1=(long int*)(s1._str)+MIN(s1._strlen/8,seqlen/16);
  long int *p1=(long int*)(s1._str)+(s/16);
  long int *p2=(long int*)(s2._str)+(s/16);
  if (p1<ep1){
    switch (s%16){
     case 1:
      dist_nogap_inc(*p1,*p2,b4_m1,count,len);
     case 2:
      dist_nogap_inc(*p1,*p2,b4_m2,count,len);
     case 3:
      dist_nogap_inc(*p1,*p2,b4_m3,count,len);
     case 4:
      dist_nogap_inc(*p1,*p2,b4_m4,count,len);
     case 5:
      dist_nogap_inc(*p1,*p2,b4_m5,count,len);
     case 6:
      dist_nogap_inc(*p1,*p2,b4_m6,count,len);
     case 7:
      dist_nogap_inc(*p1,*p2,b4_m7,count,len);
     case 8:
      dist_nogap_inc(*p1,*p2,b4_m8,count,len);
     case 9:
      dist_nogap_inc(*p1,*p2,b4_m9,count,len);
     case 10:
      dist_nogap_inc(*p1,*p2,b4_m10,count,len);
     case 11:
      dist_nogap_inc(*p1,*p2,b4_m11,count,len);
     case 12:
      dist_nogap_inc(*p1,*p2,b4_m12,count,len);
     case 13:
      dist_nogap_inc(*p1,*p2,b4_m13,count,len);
     case 14:
      dist_nogap_inc(*p1,*p2,b4_m14,count,len);
     case 15:
      dist_nogap_inc(*p1,*p2,b4_m15,count,len);
      ++p1;
      ++p2;
    }
  }

  for (; p1<ep1; ++p1,++p2){
    dist_nogap_inc(*p1,*p2,b4_m0,count,len);
    dist_nogap_inc(*p1,*p2,b4_m1,count,len);
    dist_nogap_inc(*p1,*p2,b4_m2,count,len);
    dist_nogap_inc(*p1,*p2,b4_m3,count,len);
    dist_nogap_inc(*p1,*p2,b4_m4,count,len);
    dist_nogap_inc(*p1,*p2,b4_m5,count,len);
    dist_nogap_inc(*p1,*p2,b4_m6,count,len);
    dist_nogap_inc(*p1,*p2,b4_m7,count,len);
    dist_nogap_inc(*p1,*p2,b4_m8,count,len);
    dist_nogap_inc(*p1,*p2,b4_m9,count,len);
    dist_nogap_inc(*p1,*p2,b4_m10,count,len);
    dist_nogap_inc(*p1,*p2,b4_m11,count,len);
    dist_nogap_inc(*p1,*p2,b4_m12,count,len);
    dist_nogap_inc(*p1,*p2,b4_m13,count,len);
    dist_nogap_inc(*p1,*p2,b4_m14,count,len);
    dist_nogap_inc(*p1,*p2,b4_m15,count,len);
  }

  switch (seqlen%16){
   case 15:
    dist_nogap_inc(*p1,*p2,b4_m14,count,len);
   case 14:
    dist_nogap_inc(*p1,*p2,b4_m13,count,len);
   case 13:
    dist_nogap_inc(*p1,*p2,b4_m12,count,len);
   case 12:
    dist_nogap_inc(*p1,*p2,b4_m11,count,len);
   case 11:
    dist_nogap_inc(*p1,*p2,b4_m10,count,len);
   case 10:
    dist_nogap_inc(*p1,*p2,b4_m9,count,len);
   case 9:
    dist_nogap_inc(*p1,*p2,b4_m8,count,len);
   case 8:
    dist_nogap_inc(*p1,*p2,b4_m7,count,len);
   case 7:
    dist_nogap_inc(*p1,*p2,b4_m6,count,len);
   case 6:
    dist_nogap_inc(*p1,*p2,b4_m5,count,len);
   case 5:
    dist_nogap_inc(*p1,*p2,b4_m4,count,len);
   case 4:
    dist_nogap_inc(*p1,*p2,b4_m3,count,len);
   case 3:
    dist_nogap_inc(*p1,*p2,b4_m2,count,len);
   case 2:
    dist_nogap_inc(*p1,*p2,b4_m1,count,len);
   case 1:
    dist_nogap_inc(*p1,*p2,b4_m0,count,len);
  }
  return((float)count/(float)len);
}

/*
inline void dist_nogapsingle_inc(long int a1,long int a2,long int mask,int& count,int& len,int& gapmiss){
  if ((a1&mask)==mask && (a2&mask)==mask)
    { --len; return; }

  if ((a1&mask)==(a2&mask))
    { ++count; gapmiss=0; return; }

  if ((a1&mask)==mask) {
     if (gapmiss==1) { --len; return; }
     gapmiss=1;
     return;
  }
  if ((a2&mask)==mask) {
     if (gapmiss==2) { --len; return; }
     gapmiss=2;
     return;
  }
}
*/

inline float dist_nogapsingle_short_compressed(const estr& s1,const estr& s2,int s,int seqlen) // count a continuous gap as a single mismatch, gap-gap does not count
{
  int gapmiss=0;
  int len=seqlen-s;
  int count=0;
  long int *ep1=(long int*)(s1._str)+MIN(s1._strlen/8,seqlen/16);
  long int *p1=(long int*)(s1._str)+(s/16);
  long int *p2=(long int*)(s2._str)+(s/16);
  if (p1<ep1){
    switch (s%16){
     case 1:
      dist_nogapsingle_inc(*p1,*p2,b4_m1,count,len,gapmiss);
     case 2:
      dist_nogapsingle_inc(*p1,*p2,b4_m2,count,len,gapmiss);
     case 3:
      dist_nogapsingle_inc(*p1,*p2,b4_m3,count,len,gapmiss);
     case 4:
      dist_nogapsingle_inc(*p1,*p2,b4_m4,count,len,gapmiss);
     case 5:
      dist_nogapsingle_inc(*p1,*p2,b4_m5,count,len,gapmiss);
     case 6:
      dist_nogapsingle_inc(*p1,*p2,b4_m6,count,len,gapmiss);
     case 7:
      dist_nogapsingle_inc(*p1,*p2,b4_m7,count,len,gapmiss);
     case 8:
      dist_nogapsingle_inc(*p1,*p2,b4_m8,count,len,gapmiss);
     case 9:
      dist_nogapsingle_inc(*p1,*p2,b4_m9,count,len,gapmiss);
     case 10:
      dist_nogapsingle_inc(*p1,*p2,b4_m10,count,len,gapmiss);
     case 11:
      dist_nogapsingle_inc(*p1,*p2,b4_m11,count,len,gapmiss);
     case 12:
      dist_nogapsingle_inc(*p1,*p2,b4_m12,count,len,gapmiss);
     case 13:
      dist_nogapsingle_inc(*p1,*p2,b4_m13,count,len,gapmiss);
     case 14:
      dist_nogapsingle_inc(*p1,*p2,b4_m14,count,len,gapmiss);
     case 15:
      dist_nogapsingle_inc(*p1,*p2,b4_m15,count,len,gapmiss);
      ++p1;
      ++p2;
    }
  }

  for (; p1<ep1; ++p1,++p2){
    dist_nogapsingle_inc(*p1,*p2,b4_m0,count,len,gapmiss);
    dist_nogapsingle_inc(*p1,*p2,b4_m1,count,len,gapmiss);
    dist_nogapsingle_inc(*p1,*p2,b4_m2,count,len,gapmiss);
    dist_nogapsingle_inc(*p1,*p2,b4_m3,count,len,gapmiss);
    dist_nogapsingle_inc(*p1,*p2,b4_m4,count,len,gapmiss);
    dist_nogapsingle_inc(*p1,*p2,b4_m5,count,len,gapmiss);
    dist_nogapsingle_inc(*p1,*p2,b4_m6,count,len,gapmiss);
    dist_nogapsingle_inc(*p1,*p2,b4_m7,count,len,gapmiss);
    dist_nogapsingle_inc(*p1,*p2,b4_m8,count,len,gapmiss);
    dist_nogapsingle_inc(*p1,*p2,b4_m9,count,len,gapmiss);
    dist_nogapsingle_inc(*p1,*p2,b4_m10,count,len,gapmiss);
    dist_nogapsingle_inc(*p1,*p2,b4_m11,count,len,gapmiss);
    dist_nogapsingle_inc(*p1,*p2,b4_m12,count,len,gapmiss);
    dist_nogapsingle_inc(*p1,*p2,b4_m13,count,len,gapmiss);
    dist_nogapsingle_inc(*p1,*p2,b4_m14,count,len,gapmiss);
    dist_nogapsingle_inc(*p1,*p2,b4_m15,count,len,gapmiss);
  }

  switch (seqlen%16){
   case 15:
    dist_nogapsingle_inc(*p1,*p2,b4_m14,count,len,gapmiss);
   case 14:
    dist_nogapsingle_inc(*p1,*p2,b4_m13,count,len,gapmiss);
   case 13:
    dist_nogapsingle_inc(*p1,*p2,b4_m12,count,len,gapmiss);
   case 12:
    dist_nogapsingle_inc(*p1,*p2,b4_m11,count,len,gapmiss);
   case 11:
    dist_nogapsingle_inc(*p1,*p2,b4_m10,count,len,gapmiss);
   case 10:
    dist_nogapsingle_inc(*p1,*p2,b4_m9,count,len,gapmiss);
   case 9:
    dist_nogapsingle_inc(*p1,*p2,b4_m8,count,len,gapmiss);
   case 8:
    dist_nogapsingle_inc(*p1,*p2,b4_m7,count,len,gapmiss);
   case 7:
    dist_nogapsingle_inc(*p1,*p2,b4_m6,count,len,gapmiss);
   case 6:
    dist_nogapsingle_inc(*p1,*p2,b4_m5,count,len,gapmiss);
   case 5:
    dist_nogapsingle_inc(*p1,*p2,b4_m4,count,len,gapmiss);
   case 4:
    dist_nogapsingle_inc(*p1,*p2,b4_m3,count,len,gapmiss);
   case 3:
    dist_nogapsingle_inc(*p1,*p2,b4_m2,count,len,gapmiss);
   case 2:
    dist_nogapsingle_inc(*p1,*p2,b4_m1,count,len,gapmiss);
   case 1:
    dist_nogapsingle_inc(*p1,*p2,b4_m0,count,len,gapmiss);
  }
  return((float)count/(float)len);
}



void calc_best_dist(const estr& shortseq,const estrarray& seqs,int seqlen,int& bestmatch,double& bestdist)
{
  bestmatch=-1; bestdist=0.0;

  int s,e;
  for (s=0; s<shortseq.len() && (unsigned char)shortseq[s]==0xFFu; ++s);
  e=shortseq.len()-1;
  if (shortseq.len()>0 && shortseq.len()%2==1 && (unsigned char)shortseq[e]&0x0F==0x0F) --e;
  for (; e>0 && (unsigned char)shortseq[e]==0xFFu; --e);
  if (s>=e) return; 
  
//  cout << "s: " << s << " e: " << e << " len: "<<shortseq.len() << endl;

  double tmpdist;
  int i;
  for (i=0; i<seqs.size(); ++i){
    tmpdist=dist_short_compressed(shortseq,seqs.values(i),s,e);
    if (tmpdist>bestdist){ bestdist=tmpdist; bestmatch=i; }
  }
}

void calc_best_dist_ref_profile5_task(int s,int e,eintarray& otuarr,efloatarray& distarr,int n,int tn)
{
  double tmpdist;
  int j,l,k;

  eintarray tmpotu;
  float bestotu;

  int start=(n*oturef.size())/tn;
  int end=((n+1)*oturef.size())/tn;

  for (l=start; l<end; ++l){
    lassert(oturef[l]==-1);
    tmpotu.init(otucount.size(),0);
    for (j=0; j<arr.size(); ++j){
      if (oturef[l]==j) continue;
      tmpdist=dist_nogap_short_compressed(arr[oturef[l]],arr[j],s,e);
      if (tmpdist>distarr[l]){ // if we find a sequence belonging to an otu with a closer sequence, chose that one
        distarr[l]=tmpdist;
        otuarr[l]=seqotu[arr.keys(j)];
        tmpotu.init(otucount.size(),0);
      }
      if (tmpdist==distarr[l]){ // here we keep track of how many best matching sequences each otu has
        ++tmpotu[ seqotu[ arr.keys(j) ] ]; 
      }
    }
    bestotu=0.0;
    float tmpotufrac;
    for (j=0; j<tmpotu.size(); ++j){
//      tmpotufrac=tmpotu[j];
      int otuc=otucount[j];
      if (j==l) --otuc;
      tmpotufrac=float(tmpotu[j])/otuc;
      if (tmpotufrac>bestotu){
        bestotu=tmpotufrac;
        otuarr[l]=j;
      }
    }
  }
}

void calc_best_dist_ref_profile4_task(int s,int e,eintarray& otuarr,efloatarray& distarr,int n,int tn)
{
  double tmpdist;
  int j,l,k;

  eintarray tmpotu;
  int maxotu;

  int start=(n*oturef.size())/tn;
  int end=((n+1)*oturef.size())/tn;

  for (l=start; l<end; ++l){
    lassert(oturef[l]==-1);
    tmpotu.init(otucount.size(),0);
    maxotu=0;
    for (j=0; j<arr.size(); ++j){
      if (oturef[l]==j) continue;
      tmpdist=dist_nogap_short_compressed(arr[oturef[l]],arr[j],s,e);
      if (tmpdist>distarr[l]){ // if we find a sequence belonging to an otu with a closer sequence, chose that one
        distarr[l]=tmpdist;
        otuarr[l]=seqotu[arr.keys(j)];
        maxotu=0;
        tmpotu.init(otucount.size(),0);
      }
      if (tmpdist==distarr[l]){ // here we keep track of how many best matching sequences each otu has
        ++tmpotu[ seqotu[ arr.keys(j) ] ]; 
        if (tmpotu[ seqotu[ arr.keys(j) ] ] > maxotu){ // here we chose the otu with the most matching best sequences
          maxotu=tmpotu[ seqotu[ arr.keys(j) ] ];
          otuarr[l]=seqotu[arr.keys(j)];
        }
      }
    }
  }
}

void calc_best_dist_ref_profile3_task(int s,int e,eintarray& otuarr,efloatarray& distarr,int n,int tn)
{
  double tmpdist;
  int l,j,k;

  int start=(n*oturef.size())/tn;
  int end=((n+1)*oturef.size())/tn;

  for (l=start; l<end; ++l){
    efloatarray tmpotudist;
    tmpotudist.init(otucount.size(),0.0);

    for (j=0; j<arr.size(); ++j){
      if (oturef[l]==j) continue;
      tmpdist=dist_nogap_short_compressed(arr[oturef[l]],arr[j],s,e);
      tmpotudist[ seqotu[ arr.keys(j) ] ]+=tmpdist;
    }
    for (j=0; j<tmpotudist.size(); ++j){
      int otuc=otucount[j];
      if (l == j) --otuc;
      if (otuc==0) continue;
      tmpotudist[j]=tmpotudist[j]/otuc;
      if (tmpotudist[j]>distarr[l]){
        distarr[l]=tmpotudist[j];
        otuarr[l]=j;
      }
    }
  }
}

void calc_best_dist_ref_profile2_task(int s,int e,eintarray& otuarr,efloatarray& distarr,int n,int tn)
{
  double tmpdist;
  int l,j,k;

  int start=(n*oturef.size())/tn;
  int end=((n+1)*oturef.size())/tn;

  for (l=start; l<end; ++l){
    efloatarray tmpotudist;
    tmpotudist.init(otucount.size(),1.0);

    for (j=0; j<arr.size(); ++j){
      if (oturef[l]==j) continue;
      tmpdist=dist_nogap_short_compressed(arr[oturef[l]],arr[j],s,e);
      if (tmpdist<tmpotudist[ seqotu[ arr.keys(j) ] ])
        tmpotudist[ seqotu[ arr.keys(j) ] ]=tmpdist;
    }
    for (j=0; j<tmpotudist.size(); ++j){
      if (otucount[j]==1 && l == j) continue;
      if (tmpotudist[j]>distarr[l]){
        distarr[l]=tmpotudist[j]; otuarr[l]=j;
      }
    }
  }
}





void calc_best_dist_ref_profile5(int s,int e,eintarray& otuarr,efloatarray& distarr)
{
  double tmpdist;
  int j,l,k;

  eintarray tmpotu;
  float bestotu;

  for (l=0; l<oturef.size(); ++l){
    lassert(oturef[l]==-1);
    tmpotu.init(otucount.size(),0);
    for (j=0; j<arr.size(); ++j){
      if (oturef[l]==j) continue;
      tmpdist=dist_nogapsingle_short_compressed(arr[oturef[l]],arr[j],s,e);
      if (tmpdist>distarr[l]){ // if we find a sequence belonging to an otu with a closer sequence, chose that one
        distarr[l]=tmpdist;
        otuarr[l]=seqotu[arr.keys(j)];
        tmpotu.init(otucount.size(),0);
      }
      if (tmpdist==distarr[l]){ // here we keep track of how many best matching sequences each otu has
        ++tmpotu[ seqotu[ arr.keys(j) ] ]; 
      }
    }
    bestotu=0.0;
    float tmpotufrac;
    for (j=0; j<tmpotu.size(); ++j){
//      tmpotufrac=tmpotu[j];
      tmpotufrac=float(tmpotu[j])/otucount[l];
      if (tmpotufrac>bestotu){
        bestotu=tmpotufrac;
        otuarr[l]=j;
      }
    }
  }
}


void calc_best_dist_ref_profile4(int s,int e,eintarray& otuarr,efloatarray& distarr)
{
  double tmpdist;
  int j,l,k;

  otuarr.init(oturef.size(),-1);
  distarr.init(oturef.size(),0.0);

  eintarray tmpotu;
  int maxotu;

  for (l=0; l<oturef.size(); ++l){
    lassert(oturef[l]==-1);
    tmpotu.init(otucount.size(),0);
    maxotu=0;
    for (j=0; j<arr.size(); ++j){
      if (oturef[l]==j) continue;
      tmpdist=dist_nogapsingle_short_compressed(arr[oturef[l]],arr[j],s,e);
      if (tmpdist>distarr[l]){ // if we find a sequence belonging to an otu with a closer sequence, chose that one
        distarr[l]=tmpdist;
        otuarr[l]=seqotu[arr.keys(j)];
        maxotu=0;
        tmpotu.init(otucount.size(),0);
      }
      if (tmpdist==distarr[l]){ // here we keep track of how many best matching sequences each otu has
        ++tmpotu[ seqotu[ arr.keys(j) ] ]; 
        if (tmpotu[ seqotu[ arr.keys(j) ] ] > maxotu){ // here we chose the otu with the most matching best sequences
          maxotu=tmpotu[ seqotu[ arr.keys(j) ] ];
          otuarr[l]=seqotu[arr.keys(j)];
        }
      }
    }
  }
}


void calc_best_dist_profile4(int s,int e,eintarray& otuarr,efloatarray& distarr)
{
  double tmpdist;
  int i,j,k;

  otuarr.init(arr.size(),-1);
  distarr.init(arr.size(),0.0);

  eintarray tmpotu;
  int maxotu;

  for (i=0; i<arr.size(); ++i){
    tmpotu.init(otucount.size(),0);
    maxotu=0;
    for (j=0; j<arr.size(); ++j){
      if (i==j) continue;
      tmpdist=dist_nogapsingle_short_compressed(arr[i],arr[j],s,e);
      if (tmpdist>distarr[i]){ // if we find a sequence belonging to an otu with a closer sequence, chose that one
        distarr[i]=tmpdist;
        otuarr[i]=seqotu[arr.keys(j)];
        maxotu=0;
        tmpotu.init(otucount.size(),0);
      }
      if (tmpdist==distarr[i]){ // here we keep track of how many best matching sequences each otu has
        ++tmpotu[ seqotu[ arr.keys(j) ] ]; 
        if (tmpotu[ seqotu[ arr.keys(j) ] ] > maxotu){ // here we chose the otu with the most matching best sequences
          maxotu=tmpotu[ seqotu[ arr.keys(j) ] ];
          otuarr[i]=seqotu[arr.keys(j)];
        }
      }
    }
  }
}

//template <float (*F)(const estr&,const estr&,int,int)>
void calc_best_dist_profile3(int s,int e,eintarray& otuarr,efloatarray& distarr)
{
  double tmpdist;
  int i,j,k;

  otuarr.init(arr.size(),-1);
  distarr.init(arr.size(),0.0);

  for (i=0; i<arr.size(); ++i){
    efloatarray tmpotudist;
    tmpotudist.init(otucount.size(),0.0);

    for (j=0; j<arr.size(); ++j){
      if (i==j) continue;
      tmpdist=dist_nogapsingle_short_compressed(arr[i],arr[j],s,e);
      tmpotudist[ seqotu[ arr.keys(j) ] ]+=tmpdist;
    }
    for (j=0; j<tmpotudist.size(); ++j){
      int otuc=otucount[j];
      if (seqotu[ arr.keys(i) ] == j) --otuc;
      if (tmpotudist[j]/otuc>distarr[i]){
        distarr[i]=tmpotudist[j]/otuc; otuarr[i]=j;
      }
    }
  }
}

void calc_best_dist_profile2(int s,int e,eintarray& otuarr,efloatarray& distarr)
{
  double tmpdist;
  int i,j,k;

  otuarr.init(arr.size(),-1);
  distarr.init(arr.size(),0.0);

  for (i=0; i<arr.size(); ++i){
    efloatarray tmpotudist;
    tmpotudist.init(otucount.size(),1.0);

    for (j=0; j<arr.size(); ++j){
      if (i==j) continue;
      tmpdist=dist_nogapsingle_short_compressed(arr[i],arr[j],s,e);
      if (tmpdist<tmpotudist[ seqotu[ arr.keys(j) ] ])
        tmpotudist[ seqotu[ arr.keys(j) ] ]=tmpdist;
    }
    for (j=0; j<tmpotudist.size(); ++j){
      if (otucount[j]==1 && seqotu[ arr.keys(i) ] == j) continue;
      if (tmpotudist[j]>distarr[i]){
        distarr[i]=tmpotudist[j]; otuarr[i]=j;
      }
    }
  }
}

void calc_best_dist_profile(int s,int e,eintarray& otuarr,efloatarray& distarr)
{
  double tmpdist;
  int i,j,k;

  otuarr.init(arr.size(),-1);
  distarr.init(arr.size(),0.0);

  for (i=0; i<arr.size(); ++i){
    for (j=0; j<arr.size(); ++j){
      if (i==j) continue;
      tmpdist=dist_nogapsingle_short_compressed(arr[i],arr[j],s,e);
      if (tmpdist>distarr[i]){
        distarr[i]=tmpdist;
        otuarr[i]=seqotu[arr.keys(j)];
      }
    }
  }
}

template <float (*F)(const estr&,const estr&,int,int)>
void calcProfile(int winlen,earray<efloatarray>& res)
{
  winlen=winlen/2;
  double tmpdist;
  int i,j,k;

  res.init(seqlen/winlen);

  for (i=0; i<arr.size(); ++i){
    for (j=i+1; j<arr.size(); ++j){
      tmpdist=F(arr[i],arr[j],0,seqlen);
      if (tmpdist<0.80) continue;

      res[0].add(tmpdist);
//      cout << tmpdist;

      for (k=1; k*winlen<seqlen-winlen; ++k){
//      for (k=winlen; k<seqlen-winlen; k+=winlen){
        tmpdist=F(arr[i],arr[j],k*winlen-winlen,k*winlen+winlen);
        res[k].add(tmpdist);
//        cout << " " << tmpdist;
      }
//      cout << endl;
    }
  }
}

int emain()
{ 
  cout << "# " << date() << endl;
  cout << "# " << args2str(argvc,argv) << endl;

  eoption<efunc> dfunc;
  dfunc.choice=0;
  dfunc.add("nogapsingle",calcProfile<dist_nogapsingle_short_compressed>);
  dfunc.add("nogap",calcProfile<dist_nogap_short_compressed>);
  dfunc.add("gap",calcProfile<dist_short_compressed>);
  dfunc.add("tamura",calcProfile<dist_tamura_short_compressed>);
  epregisterClass(eoption<efunc>);
  epregisterClassMethod2(eoption<efunc>,operator=,int,(const estr& val));
  epregister(dfunc);

  eoption<efunc> match;
  match.choice=0;
  match.add("avg",calc_best_dist_ref_profile3_task);
  match.add("bestcount",calc_best_dist_ref_profile4_task);
  match.add("bestfrac",calc_best_dist_ref_profile5_task);
  match.add("bestmin",calc_best_dist_ref_profile2_task);
  epregister(match);

  estr otufile;
  epregister(otufile);

  int winlen=150;
  epregister(winlen);
  bool profile=false;
  epregister(profile);
  int ncpus=4;
  epregister(ncpus);

  eparseArgs(argvc,argv);


//  cout << "# distance function: " << dfunc.key() << endl;

  ldieif(argvc<3 && !profile && otufile.len()==0 || argvc<2,"syntax: "+efile(argv[0]).basename()+" [--profile <true|false>] <long-seqs> <short-seqs>");

  int i,j,k;
  estrhashof<int> arrind;
  load_seqs_compressed(argv[1],arr,arrind,seqlen);

  if (profile){
    earray<efloatarray> res;
    dfunc.value()(evararray(winlen,res));
    for (k=1; k<res.size(); ++k){
      eintarray ind(iheapsort(res[k]));

      int count=0;
      for (i=0; i<res[0].size(); ++i){
        if (res[0][i]>=0.97)
          ++count;
      }

      float tdist;
      int ccount;
      for (i=res[0].size()-2; i>=0; --i){
        for (tdist=res[k][ind[i]]; res[k][ind[i]]>=tdist && i>=0; --i);
        ccount=0;
        for (j=res[0].size()-1; j>i; --j){
          if (res[0][ind[j]]>=0.97)
            ++ccount;
        }
        cout << k << " " << tdist << " " << float(ccount)/(res[0].size()-i-1) << " " << float(res[0].size()-i-1-ccount)/(i+1) << endl;
      }
    }
  }else if (otufile.len()){
    cout << "# reading otufile: " << otufile << endl;
    efile f(otufile);
    estr line;
    int otu=-1;
    while (f.readln(line)){
      if (line.len()==0 || line[0]=='#') continue;
      if (line[0]=='>') { ++otu; otucount.add(0); oturef.add(-1); continue; }
      seqotu.add(line,otu);
      ++otucount[otu];
      if (oturef[otu]==-1){
        ldieif(!arrind.exists(line),"line not found: " +line);
        oturef[otu]=arrind[line];
      }
    }
    int singletons=0;
    for (i=0; i<otucount.size(); ++i){
      if (otucount[i]==1)
        ++singletons;
    }
    cout << "# otus: " << otucount.size() << endl;
    cout << "# singletons: " << singletons << endl;

    etaskman taskman;
    taskman.createThread(ncpus);

    winlen=winlen/2;
    for (k=0; k<seqlen/winlen; ++k){
      eintarray otuarr;
      efloatarray distarr;
//      taskman.addTask(calc_best_dist_ref_profile4(k*winlen,k*winlen+2*winlen,otuarr,distarr);
      otuarr.init(oturef.size(),-1);
      distarr.init(oturef.size(),0.0);
      for (i=0; i<ncpus; ++i)
        taskman.addTask(match.value(),evararray(k*winlen,k*winlen+2*winlen,otuarr,distarr,(const int&)i,(const int&)ncpus));
//        taskman.addTask(calc_best_dist_ref_profile5_task,evararray(k*winlen,k*winlen+2*winlen,otuarr,distarr,(const int&)i,(const int&)ncpus));
      taskman.wait();
//      calc_best_dist_ref_profile4(k*winlen,k*winlen+2*winlen,otuarr,distarr);

      eintarray sind(iheapsort(distarr));

      int alltp=0;
      int alltn=0;
      for (j=distarr.size()-1; j>=0; --j){
//        if (otuarr[sind[j]]==seqotu[arr.keys(sind[j])]) ++alltp;
//        else if (otucount[seqotu[arr.keys(sind[j])]]>1) ++alltn;
        if (otuarr[sind[j]]==seqotu[arr.keys(oturef[sind[j]])]) ++alltp;
        else if (otucount[seqotu[arr.keys(oturef[sind[j]])]]>1) ++alltn;
      }
      cout << "# otus: " << otucount.size() << endl;
      cout << "# seqs: " << distarr.size() << endl;
      cout << "# true positives: " << alltp << endl;
      cout << "# true negatives: " << alltn << endl;
      cout << "# singletons: " << singletons << endl;
    
      float tdist;
      for (i=distarr.size()-1; i>=0; --i){
        for (tdist=distarr[sind[i]]; i>=0 && distarr[sind[i]]>=tdist; --i);
        int tp=0;
        int fp=0;
        for (j=distarr.size()-1; j>i; --j){
//          if (otuarr[sind[j]]==seqotu[arr.keys(sind[j])]) ++tp;
//          else if (otucount[seqotu[arr.keys(sind[j])]]>1) ++fp;
          if (otuarr[sind[j]]==seqotu[arr.keys(oturef[sind[j]])]) ++tp;
          else if (otucount[seqotu[arr.keys(oturef[sind[j]])]]>1) ++fp;
        }
        cout << k << " " << tdist << " " << float(tp)/alltp << " " << float(fp)/(distarr.size()-alltp-singletons) << " " << float(distarr.size()-i-1-tp)/(distarr.size()-alltp) << endl;
//        cout << k << " " << tdist << " " << float(tp)/(distarr.size()-singletons) << " " << float(distarr.size()-i-1-tp)/(distarr.size()-alltp) << endl;
      }
    }
  }else{
    load_short_compressed(argv[2],arrshort,seqlen);
  
    cout << "# computing distances" << endl;
    int bestmatch;
    double bestdist;
    for (i=0; i<arrshort.size(); ++i){
      calc_best_dist(arrshort.values(i),arr,seqlen,bestmatch,bestdist);
      if (bestmatch==-1) continue;
      cout << arrshort.keys(i) << " " << bestdist << " " << arr.keys(bestmatch) << endl;
    }
  }

  return(0);
}

