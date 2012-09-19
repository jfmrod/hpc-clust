#include <eutils/emain.h>
#include <eutils/efile.h>
#include <eutils/eheap.h>
#include <eutils/etime.h>
#include <eutils/etimer.h>
#include <eutils/eoption.h>
#include <eutils/ethread.h>

#include "cluster-common.h"

class eotuinfo
{
 public:
  eintarray seqs;
  float mindist;
  float maxdist;
  float avgdist;
};

ostream& operator<<(ostream& stream,const eotuinfo& otuinfo)
{
  stream<<"[otuinfo size: "<<otuinfo.seqs.size()<<" min/avg/max: "<<otuinfo.mindist<<"/"<<otuinfo.avgdist<<"/"<<otuinfo.maxdist<<"]";
  return(stream);
}

class ematchinfo
{
 public:
  int otu;
  float dist;
  float dist2;
  float mindist;
  float avgdist;
  float maxdist;
  int eqcount;
  int bestcount;
  eintarray seqlist;
  eintarray seqcluster;
  float cdist;

  bool operator<(const ematchinfo& minfo);
};

bool ematchinfo::operator<(const ematchinfo& minfo)
{
  return(dist < minfo.dist);
}

ostream& operator<<(ostream& stream,const ematchinfo& matchinfo)
{
  stream<<"[matchinfo dist: "<<matchinfo.dist<<" dist2: "<<matchinfo.dist2 << " mindist: " << matchinfo.mindist <<" eqcount: "<<matchinfo.eqcount<<"]";
  return(stream);
}


estrarray arr;
ebasicarray<eshortseq> arrshort;
//estrhashof<int> seqotu;
earray<eotuinfo> otuinfo;
eintarray seqotuid;
eintarray seqotuid2;
//eintarray oturef;

eseqclusterData clusterData;

int seqlen=0;
float eqthres=1.0;

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

void place_short_task(earray<ematchinfo>& matchinfo,int n,int tn)
{
  int j,l,k;

  efloatarray tmpdists;
  eintarray tmpotu;

  tmpdists.init(arr.size());

  int start=(n*arrshort.size())/tn;
  int end=((n+1)*arrshort.size())/tn;

  int s,e;

  for (l=start; l<end; ++l){
//    for (s=0; s<arrshort[l].len() && (unsigned char)arrshort[l][s]==0xFFu; ++s);
//    e=arrshort[l].len()-1;
//    if (arrshort[l].len()>0 && arrshort[l].len()%2==1 && (unsigned char)arrshort[l][e]&0x0F==0x0F) --e;
//    for (; e>0 && (unsigned char)arrshort[l][e]==0xFFu; --e);
    s=arrshort[l].b;
    e=arrshort[l].e;
    
    if (s>=e) continue;


    matchinfo[l].eqcount=0;
    matchinfo[l].bestcount=0;
    matchinfo[l].dist=0.0;
    for (j=0; j<arr.size(); ++j)
      tmpdists[j]=dist_nogap_short_compressed(arrshort[l].seq,arr[j],s,e);

    tmpotu.init(otuinfo.size(),0);
    eintarray si(iheapsort(tmpdists));
    matchinfo[l].dist=tmpdists[si[si.size()-1]];
    for (j=tmpdists.size()-1; j>=0 && tmpdists[si[j]]>=matchinfo[l].dist; --j){
      ++tmpotu[ seqotuid[si[j]] ]; 
      if (tmpotu[ seqotuid[si[j]]] > matchinfo[l].bestcount){
        matchinfo[l].bestcount=tmpotu[ seqotuid[si[j]] ];
        matchinfo[l].otu=seqotuid[si[j]];
      }
    }
    for (; j>=0 && tmpdists[si[j]]>=matchinfo[l].dist-(1.0-matchinfo[l].dist)*eqthres; --j){
      if (tmpotu[ seqotuid[si[j]] ]==0)
        ++matchinfo[l].eqcount;
      ++tmpotu[ seqotuid[si[j]] ]; 
    }
  }
}




void place_short_adaptive_task(earray<ematchinfo>& matchinfo,int n,int tn)
{
  int j,l,k;

  int start=(n*arrshort.size())/tn;
  int end=((n+1)*arrshort.size())/tn;

  float tmpdist,bestdist;
  eintarray tmpseqs;
  efloatarray tmpdists;

  tmpseqs.reserve(arrshort.size());
  tmpdists.reserve(arrshort.size());

  int s,e;
  for (l=start; l<end; ++l){
    s=arrshort[l].b;
    e=arrshort[l].e;
    
    if (s>=e) continue;

    tmpdists.clear();
    tmpseqs.clear();

    bestdist=0.0;
    for (j=0; j<arr.size(); ++j){
      tmpdist=dist_nogap_short_compressed(arrshort[l].seq,arr[j],s,e);
      if (tmpdist>bestdist)
        bestdist=tmpdist;

      if (tmpdist >= bestdist-(1.0-bestdist)*eqthres) { 
        tmpseqs.add(j);
        tmpdists.add(tmpdist);
      }
    }

    eintarray si(iheapsort(tmpdists));
    for (k=tmpdists.size()-1; k>=0 && tmpdists[si[k]]>=bestdist-(1.0-bestdist)*eqthres; --k)
      matchinfo[l].seqlist.add(si[k]);

    matchinfo[l].dist=bestdist;
    if (matchinfo[l].seqlist.size()>1) 
      clusterData.getCluster(matchinfo[l].seqlist, matchinfo[l].seqcluster, matchinfo[l].cdist);
  }
}




eintarray varr;

/*
void validate_short_adaptive_task(int s,int e,earray<ematchinfo>& matchinfo,int n,int tn)
{
  int j,l,k;

  efloatarray tmpdists;
//  eintarray tmpseqs;
//  eintarray tmpotu;
  float tmpdist,bestdist;

  tmpdists.init(arr.size());
//  tmpseqs.reserve(arr.size());

  int start=(n*varr.size())/tn;
  int end=((n+1)*varr.size())/tn;

  for (l=start; l<end; ++l){
    matchinfo[l].bestcount=0;
    matchinfo[l].dist=0.0;
    matchinfo[l].eqcount=0;
    bestdist=0.0;
    for (j=0; j<arr.size(); ++j){
      if (seqotuid2[varr[l]]==seqotuid2[j]) continue;

      tmpdists[j]=dist_nogap_short_compressed(arr[varr[l]],arr[j],s,e);
      if (tmpdists[j]>bestdist)
        bestdist=tmpdists[j];
    }

    matchinfo[l].dist=bestdist;
    matchinfo[l].mindist=bestdist;
    matchinfo[l].eqcount=0;
    eintarray si(iheapsort(tmpdists));
    matchinfo[l].otu=seqotuid[si[tmpdists.size()-1]];
    for (k=tmpdists.size()-1; k>=0; --k){
      if (seqotuid[si[k]] != matchinfo[l].otu){
//        ++matchinfo[l].eqcount;
        matchinfo[l].dist2=tmpdists[si[k]];
        break;
      }
      matchinfo[l].mindist=tmpdists[si[k]];

    }
  }
   eintarray si(iheapsort(tmpdists));
    for (k=tmpdists.size()-1; k>=0 && tmpdists[si[k]]>=bestdist-(1.0-bestdist)*eqthres; --k)
      matchinfo[l].seqlist.add(si[k]);

    matchinfo[l].dist=bestdist;
    if (matchinfo[l].seqlist.size()>1) 
      clusterData.getCluster(matchinfo[l].seqlist, matchinfo[l].seqcluster, matchinfo[l].cdist);
  }
}

*/



void validate_short_task2(int s,int e,earray<ematchinfo>& matchinfo,int n,int tn)
{
  int j,l,k;

  efloatarray tmpdists;
//  eintarray tmpseqs;
//  eintarray tmpotu;
  float tmpdist,bestdist;

  tmpdists.init(arr.size());
//  tmpseqs.reserve(arr.size());

  int start=(n*varr.size())/tn;
  int end=((n+1)*varr.size())/tn;

  for (l=start; l<end; ++l){
    matchinfo[l].bestcount=0;
    matchinfo[l].dist=0.0;
    matchinfo[l].eqcount=0;
//    tmpdists.clear();
//    tmpseqs.clear();
    bestdist=0.0;
    for (j=0; j<arr.size(); ++j){
      if (seqotuid2[varr[l]]==seqotuid2[j]) { tmpdists[j]=0.0; continue; }

      tmpdists[j]=dist_nogap_short_compressed(arr[varr[l]],arr[j],s,e);
      if (tmpdists[j]>bestdist)
        bestdist=tmpdists[j];

//      if (tmpdist >= bestdist-(1.0-bestdist)*eqthres) { 
//        tmpseqs.add(j);
//        tmpdists.add(tmpdist);
//      }
    }

    matchinfo[l].dist=bestdist;
    matchinfo[l].mindist=bestdist;
    matchinfo[l].eqcount=0;
    eintarray si(iheapsort(tmpdists));
    matchinfo[l].otu=seqotuid[si[tmpdists.size()-1]];
    for (k=tmpdists.size()-1; k>=0; --k){ //&& tmpdists[si[k]]>=bestdist-(1.0-bestdist)*eqthres; --k){
//      matchinfo[l].seqlist.add(si[k]);
      if (seqotuid[si[k]] != matchinfo[l].otu){
//        ++matchinfo[l].eqcount;
        matchinfo[l].dist2=tmpdists[si[k]];
        break;
      }
      matchinfo[l].mindist=tmpdists[si[k]];

    }
  }
/*

    tmpotu.init(otuinfo.size(),0);
    eintarray si(iheapsort(tmpdists));
    matchinfo[l].dist=tmpdists[si[si.size()-1]];
    for (j=tmpdists.size()-1; j>=0 && tmpdists[si[j]]>=matchinfo[l].dist; --j){
      ++tmpotu[ seqotuid[si[j]] ]; 
      if (tmpotu[ seqotuid[si[j]]] > matchinfo[l].bestcount){
        matchinfo[l].bestcount=tmpotu[ seqotuid[si[j]] ];
        matchinfo[l].otu=seqotuid[si[j]];
      }
    }
    for (; j>=0 && tmpdists[si[j]]>=matchinfo[l].dist-(1.0-matchinfo[l].dist)*eqthres; --j){
      if (tmpotu[ seqotuid[si[j]] ]==0)
        ++matchinfo[l].eqcount;
      ++tmpotu[ seqotuid[si[j]] ]; 
    }
  }
*/
}


void validate_short_task(int s,int e,earray<ematchinfo>& matchinfo,int n,int tn)
{
  int j,l,k;

  efloatarray tmpdists;
//  eintarray tmpseqs;
//  eintarray tmpotu;
  float tmpdist,bestdist;

  tmpdists.init(arr.size());
//  tmpseqs.reserve(arr.size());

  int start=(n*varr.size())/tn;
  int end=((n+1)*varr.size())/tn;

  for (l=start; l<end; ++l){
    matchinfo[l].bestcount=0;
    matchinfo[l].dist=0.0;
    matchinfo[l].eqcount=0;
//    tmpdists.clear();
//    tmpseqs.clear();
    bestdist=0.0;
    for (j=0; j<arr.size(); ++j){
      if (varr[l]==j) { tmpdists[j]=0.0; continue; }

      tmpdists[j]=dist_nogap_short_compressed(arr[varr[l]],arr[j],s,e);
      if (tmpdists[j]>bestdist)
        bestdist=tmpdists[j];

//      if (tmpdist >= bestdist-(1.0-bestdist)*eqthres) { 
//        tmpseqs.add(j);
//        tmpdists.add(tmpdist);
//      }
    }

    matchinfo[l].dist=bestdist;
    matchinfo[l].mindist=bestdist;
    matchinfo[l].eqcount=0;
    eintarray si(iheapsort(tmpdists));
    matchinfo[l].otu=seqotuid[si[tmpdists.size()-1]];
    for (k=tmpdists.size()-1; k>=0; --k){ //&& tmpdists[si[k]]>=bestdist-(1.0-bestdist)*eqthres; --k){
//      matchinfo[l].seqlist.add(si[k]);
      if (seqotuid[si[k]] != matchinfo[l].otu){
//        ++matchinfo[l].eqcount;
        matchinfo[l].dist2=tmpdists[si[k]];
        break;
      }
      matchinfo[l].mindist=tmpdists[si[k]];

    }
  }
/*

    tmpotu.init(otuinfo.size(),0);
    eintarray si(iheapsort(tmpdists));
    matchinfo[l].dist=tmpdists[si[si.size()-1]];
    for (j=tmpdists.size()-1; j>=0 && tmpdists[si[j]]>=matchinfo[l].dist; --j){
      ++tmpotu[ seqotuid[si[j]] ]; 
      if (tmpotu[ seqotuid[si[j]]] > matchinfo[l].bestcount){
        matchinfo[l].bestcount=tmpotu[ seqotuid[si[j]] ];
        matchinfo[l].otu=seqotuid[si[j]];
      }
    }
    for (; j>=0 && tmpdists[si[j]]>=matchinfo[l].dist-(1.0-matchinfo[l].dist)*eqthres; --j){
      if (tmpotu[ seqotuid[si[j]] ]==0)
        ++matchinfo[l].eqcount;
      ++tmpotu[ seqotuid[si[j]] ]; 
    }
  }
*/
}








void calc_best_dist_ref_profile5_task(int s,int e,earray<ematchinfo>& matchinfo,int n,int tn)
{
  double tmpdist;
  int j,l,k;

  eintarray tmpotu;
  float bestotu;

  int start=(n*otuinfo.size())/tn;
  int end=((n+1)*otuinfo.size())/tn;

  for (l=start; l<end; ++l){
    lassert(otuinfo[l].seqs.size()==0);
    matchinfo[l].dist=0.0;
    matchinfo[l].eqcount=0;
    tmpotu.init(otuinfo.size(),0);
    for (j=0; j<arr.size(); ++j){
      if (otuinfo[l].seqs[0]==j) continue;
      tmpdist=dist_nogap_short_compressed(arr[otuinfo[l].seqs[0]],arr[j],s,e);
      if (tmpdist>matchinfo[l].dist){ // if we find a sequence belonging to an otu with a closer sequence, chose that one
        matchinfo[l].dist=tmpdist;
        matchinfo[l].otu=seqotuid[j];
        tmpotu.init(otuinfo.size(),0);
      }
      if (tmpdist==matchinfo[l].dist){ // here we keep track of how many best matching sequences each otu has
        ++tmpotu[ seqotuid[j] ]; 
      }
    }
    bestotu=0.0;
    float tmpotufrac;
    for (j=0; j<tmpotu.size(); ++j){
//      tmpotufrac=tmpotu[j];
      int otuc=otuinfo[j].seqs.size();
      if (j==l) --otuc;
      tmpotufrac=float(tmpotu[j])/otuc;
      if (tmpotufrac>bestotu){
        bestotu=tmpotufrac;
        matchinfo[l].otu=j;
      }
    }
  }
}

//void calc_best_dist_ref_profile4_task(int s,int e,eintarray& otuarr,efloatarray& distarr,int n,int tn)
void calc_best_dist_ref_profile41_task(int s,int e,earray<ematchinfo>& matchinfo,int n,int tn)
{
  int j,l,k;

  efloatarray tmpdists;
  eintarray tmpotu;

  tmpdists.init(arr.size());

  int start=(n*otuinfo.size())/tn;
  int end=((n+1)*otuinfo.size())/tn;

  for (l=start; l<end; ++l){
    lassert(otuinfo[l].seqs.size()==0);
    matchinfo[l].bestcount=0;
    matchinfo[l].dist=0.0;
    for (j=0; j<arr.size(); ++j){
      if (otuinfo[l].seqs[0]==j) { tmpdists[j]=0.0; continue; }
      tmpdists[j]=dist_nogap_short_compressed(arr[otuinfo[l].seqs[0]],arr[j],s,e);
    }
    tmpotu.init(otuinfo.size(),0);
    eintarray si(iheapsort(tmpdists));
    matchinfo[l].dist=tmpdists[si[si.size()-1]];
    for (j=tmpdists.size()-1; j>=0 && tmpdists[si[j]]>=matchinfo[l].dist-(1.0-matchinfo[l].dist)*0.5; --j){
      ++tmpotu[ seqotuid[si[j]] ]; 
      if (tmpotu[ seqotuid[si[j]] ] > matchinfo[l].bestcount){ // here we chose the otu with the most matching best sequences
        matchinfo[l].bestcount=tmpotu[ seqotuid[si[j]] ];
        matchinfo[l].otu=seqotuid[si[j]];
        matchinfo[l].eqcount=0;
      }else if (tmpotu[ seqotuid[si[j]]] == matchinfo[l].bestcount)
        ++matchinfo[l].eqcount;
    }
  }
}


void calc_best_dist_ref_profile42_task(int s,int e,earray<ematchinfo>& matchinfo,int n,int tn)
{
  int j,l,k;

  efloatarray tmpdists;
  eintarray tmpotu;

  tmpdists.init(arr.size());

  int start=(n*otuinfo.size())/tn;
  int end=((n+1)*otuinfo.size())/tn;

  for (l=start; l<end; ++l){
    lassert(otuinfo[l].seqs.size()==0);
    matchinfo[l].bestcount=0;
    matchinfo[l].dist=0.0;
    for (j=0; j<arr.size(); ++j){
      if (otuinfo[l].seqs[0]==j) { tmpdists[j]=0.0; continue; }
      tmpdists[j]=dist_nogap_short_compressed(arr[otuinfo[l].seqs[0]],arr[j],s,e);
    }
    tmpotu.init(otuinfo.size(),0);
    eintarray si(iheapsort(tmpdists));
    matchinfo[l].dist=tmpdists[si[si.size()-1]];
    for (j=tmpdists.size()-1; j>=0 && tmpdists[si[j]]>=matchinfo[l].dist-(1.0-matchinfo[l].dist)*eqthres; --j){
      ++tmpotu[ seqotuid[si[j]] ]; 
    }
    eintarray si2(iheapsort(tmpotu));

    matchinfo[l].eqcount=0;
    matchinfo[l].bestcount=tmpotu[ si2[tmpotu.size()-1] ];
    matchinfo[l].otu=si2[tmpotu.size()-1];
//    for (j=tmpotu.size()-2; j>=0 && tmpotu[si2[j]]>=matchinfo[l].bestcount-10 && tmpotu[si2[j]]>0; --j)
    for (j=tmpotu.size()-2; j>=0 && tmpotu[si2[j]]>0; --j)
      ++matchinfo[l].eqcount;
  }
}

void calc_best_dist_ref_profile43_task(int s,int e,earray<ematchinfo>& matchinfo,int n,int tn)
{
  int j,l,k;

  efloatarray tmpdists;
  eintarray tmpotu;

  tmpdists.init(arr.size());

  int start=(n*otuinfo.size())/tn;
  int end=((n+1)*otuinfo.size())/tn;

  for (l=start; l<end; ++l){
    lassert(otuinfo[l].seqs.size()==0);
    matchinfo[l].bestcount=0;
    matchinfo[l].dist=0.0;
    for (j=0; j<arr.size(); ++j){
      if (otuinfo[l].seqs[0]==j) { tmpdists[j]=0.0; continue; }
      tmpdists[j]=dist_nogap_short_compressed(arr[otuinfo[l].seqs[0]],arr[j],s,e);
    }
    tmpotu.init(otuinfo.size(),0);
    eintarray si(iheapsort(tmpdists));
    matchinfo[l].dist=tmpdists[si[si.size()-1]];
    for (j=tmpdists.size()-1; j>=0 && tmpdists[si[j]]>=matchinfo[l].dist; --j){
      ++tmpotu[ seqotuid[si[j]] ]; 
      if (tmpotu[ seqotuid[si[j]]] > matchinfo[l].bestcount){
        matchinfo[l].bestcount=tmpotu[ seqotuid[si[j]] ];
        matchinfo[l].otu=seqotuid[si[j]];
      }
    }
    for (; j>=0 && tmpdists[si[j]]>=matchinfo[l].dist-(1.0-matchinfo[l].dist)*eqthres; --j)
      ++tmpotu[ seqotuid[si[j]] ]; 

    eintarray si2(iheapsort(tmpotu));

    matchinfo[l].eqcount=0;
//    for (j=tmpotu.size()-2; j>=0 && tmpotu[si2[j]]>=matchinfo[l].bestcount-10 && tmpotu[si2[j]]>0; --j)
    for (j=tmpotu.size()-2; j>=0 && tmpotu[si2[j]]>0; --j)
      ++matchinfo[l].eqcount;
  }
}

void calc_best_dist_ref_profile4_task(int s,int e,earray<ematchinfo>& matchinfo,int n,int tn)
{
  double tmpdist;
  int j,l,k;

  eintarray tmpotu;

  int start=(n*otuinfo.size())/tn;
  int end=((n+1)*otuinfo.size())/tn;

  for (l=start; l<end; ++l){
    lassert(otuinfo[l].seqs.size()==0);
    tmpotu.init(otuinfo.size(),0);
    matchinfo[l].bestcount=0;
    matchinfo[l].dist=0.0;
    for (j=0; j<arr.size(); ++j){
      if (otuinfo[l].seqs[0]==j) continue;
      tmpdist=dist_nogap_short_compressed(arr[otuinfo[l].seqs[0]],arr[j],s,e);
      if (tmpdist>matchinfo[l].dist){ // if we find a sequence belonging to an otu with a closer sequence, chose that one
        matchinfo[l].dist=tmpdist;
        matchinfo[l].otu=seqotuid[j];
        matchinfo[l].eqcount=0;
        matchinfo[l].bestcount=0;
        tmpotu.init(otuinfo.size(),0);
      }
      if (tmpdist==matchinfo[l].dist){ // here we keep track of how many best matching sequences each otu has
        ++tmpotu[ seqotuid[j] ]; 
        if (tmpotu[ seqotuid[j] ] > matchinfo[l].bestcount){ // here we chose the otu with the most matching best sequences
          matchinfo[l].bestcount=tmpotu[ seqotuid[j] ];
          matchinfo[l].otu=seqotuid[j];
          matchinfo[l].eqcount=0;
        }else if (tmpotu[ seqotuid[j]] == matchinfo[l].bestcount)
          ++matchinfo[l].eqcount;
      }
    }
  }
}

void calc_best_dist_ref_profile3_task(int s,int e,earray<ematchinfo>& matchinfo,int n,int tn)
{
  double tmpdist;
  int l,j,k;

  int start=(n*otuinfo.size())/tn;
  int end=((n+1)*otuinfo.size())/tn;

  for (l=start; l<end; ++l){
    efloatarray tmpotudist;
    tmpotudist.init(otuinfo.size(),0.0);

    for (j=0; j<arr.size(); ++j){
      if (otuinfo[l].seqs[0]==j) continue;
      tmpdist=dist_nogap_short_compressed(arr[otuinfo[l].seqs[0]],arr[j],s,e);
      tmpotudist[ seqotuid[j] ]+=tmpdist;
    }
    matchinfo[l].dist=0.0;
    matchinfo[l].eqcount=0;
    for (j=0; j<tmpotudist.size(); ++j){
      int otuc=otuinfo[j].seqs.size();
      if (l == j) --otuc;
      if (otuc==0) continue;
      tmpotudist[j]=tmpotudist[j]/otuc;
      if (tmpotudist[j]>matchinfo[l].dist){
        matchinfo[l].dist=tmpotudist[j];
        matchinfo[l].otu=j;
      }
    }
  }
}

void calc_best_dist_ref_profile2_task(int s,int e,earray<ematchinfo>& matchinfo,int n,int tn)
{
  double tmpdist;
  int l,j,k;

  int start=(n*otuinfo.size())/tn;
  int end=((n+1)*otuinfo.size())/tn;

  for (l=start; l<end; ++l){
    efloatarray tmpotudist;
    tmpotudist.init(otuinfo.size(),1.0);

    for (j=0; j<arr.size(); ++j){
      if (otuinfo[l].seqs[0]==j) continue;
      tmpdist=dist_nogap_short_compressed(arr[otuinfo[l].seqs[0]],arr[j],s,e);
      if (tmpdist<tmpotudist[ seqotuid[j] ])
        tmpotudist[ seqotuid[j] ]=tmpdist;
    }
    matchinfo[l].dist=0.0;
    matchinfo[l].eqcount=0;
    for (j=0; j<tmpotudist.size(); ++j){
      if (otuinfo[j].seqs.size()==1 && l == j) continue;
      if (tmpotudist[j]>matchinfo[l].dist){
        matchinfo[l].dist=tmpotudist[j]; matchinfo[l].otu=j;
      }
    }
  }
}


/*



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
*/

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

eintarray rnd_ind;

void calcOTUInfo_task(int s,int e,int n,int tn)
{
  int start=(n*otuinfo.size())/tn;
  int end=((n+1)*otuinfo.size())/tn;

  float tmpdist;
  int i,k,j,l;
  for (k=start; k<end; ++k){
    i=rnd_ind[k];
    otuinfo[i].maxdist=0.0; otuinfo[i].mindist=1.0; otuinfo[i].avgdist=0.0;
    for (j=1; j<otuinfo[i].seqs.size(); ++j){
      for (l=0; l<j; ++l){
        tmpdist=dist_nogap_short_compressed(arr[otuinfo[i].seqs[j]],arr[otuinfo[i].seqs[l]],s,e);
        if (tmpdist>otuinfo[i].maxdist) otuinfo[i].maxdist=tmpdist;
        if (tmpdist<otuinfo[i].mindist) otuinfo[i].mindist=tmpdist;
        otuinfo[i].avgdist+=tmpdist;
      }
    }
    otuinfo[i].avgdist=otuinfo[i].avgdist/(otuinfo[i].seqs.size()*(otuinfo[i].seqs.size()+1)/2 - otuinfo[i].seqs.size());
  }
}

eintarray sequence(int count)
{ 
  eintarray tmparr;
  int i;
  tmparr.reserve(count);
  for (i=0; i<count; ++i)
    tmparr.add(i);
  return(tmparr);
}

void rnd_permutation(eintarray& arr)
{
  int i,t;
  for (i=0; i<arr.size()-1; ++i){
    t=i+rnd.uniform()*(arr.size()-i);
    if (t!=i)
      arr.swap(i,t);
  }
}

int emain()
{ 
  cout << "# " << date() << endl;
  cout << "# " << args2str(argvc,argv) << endl;

  epregister(eqthres);
  int minotusize=1;
  epregister(minotusize);

  bool adaptive=false;
  epregister(adaptive);

  float ct=0.98;
  epregister(ct);
  float vct=0.99;
  epregister(vct);


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
  match.add("bestcount3",calc_best_dist_ref_profile42_task);
  match.add("bestcount4",calc_best_dist_ref_profile43_task);
  match.add("bestcount2",calc_best_dist_ref_profile41_task);
  match.add("bestcount",calc_best_dist_ref_profile4_task);
  match.add("avg",calc_best_dist_ref_profile3_task);
  match.add("bestfrac",calc_best_dist_ref_profile5_task);
  match.add("bestmin",calc_best_dist_ref_profile2_task);
  epregister(match);

//  estr otufile;
//  epregister(otufile);

  int winlen=150;
  epregister(winlen);
  int ncpus=4;
  epregister(ncpus);

  eparseArgs(argvc,argv);


//  cout << "# distance function: " << dfunc.key() << endl;

  ldieif(argvc<3,"syntax: "+efile(argv[0]).basename()+" <cluster-file> <long-seqs> [short-seqs]");

  int i,j,k;
  estrhashof<int> arrind;
  load_seqs_compressed(argv[2],arr,arrind,seqlen);

  clusterData.load(estr(argv[1]),arr.size());
  seqotuid.init(arr.size(),-1);
  seqotuid2.init(arr.size(),-1);

  clusterData.getOTU(vct,seqotuid2);

  int otucount=clusterData.getOTU(ct,seqotuid);
  otuinfo.init(otucount);
  for (i=0; i<seqotuid.size(); ++i)
    otuinfo[seqotuid[i]].seqs.add(i);

  int singletons=0;
  for (i=0; i<otuinfo.size(); ++i){
    if (otuinfo[i].seqs.size()==1)
      ++singletons;
  }
  cout << "# otus: " << otuinfo.size() << endl;
  cout << "# singletons: " << singletons << endl;

  rnd_ind=sequence(otuinfo.size());
  rnd_permutation(rnd_ind);

  etaskman taskman;
  taskman.createThread(ncpus);

  earray<ematchinfo> matchinfo;

  if (argvc==4){
    if (!adaptive){
      load_short_compressed(argv[3],arrshort);
      matchinfo.init(arrshort.size());
  
      cout << "# starting short sequence placement" << endl;
      for (i=0; i<ncpus; ++i)
        taskman.addTask(place_short_task,evararray(matchinfo,(const int&)i,(const int&)ncpus));
      taskman.wait();

      for (j=0; j<arrshort.size(); ++j){
        if (matchinfo[j].eqcount>0 || otuinfo[matchinfo[j].otu].seqs.size()<=minotusize) {
          cout << "# " << arrshort[j].name << " " << matchinfo[j].dist << " " << otuinfo[matchinfo[j].otu].seqs.size() << " " << arr.keys(otuinfo[matchinfo[j].otu].seqs[0]) << " " << matchinfo[j] << endl;
          continue;
        }
        cout << arrshort[j].name << " " << matchinfo[j].dist << " " << otuinfo[matchinfo[j].otu].seqs.size() << " " << arr.keys(otuinfo[matchinfo[j].otu].seqs[0]) << " " << matchinfo[j] << endl;
      }
    } else { // adaptive placement
      load_short_compressed(argv[3],arrshort);
      matchinfo.init(arrshort.size());
  
      cout << "# starting adaptive short sequence placement" << endl;
      for (i=0; i<ncpus; ++i)
        taskman.addTask(place_short_adaptive_task,evararray(matchinfo,(const int&)i,(const int&)ncpus));
      taskman.wait();

      for (j=0; j<matchinfo.size(); ++j){
        cout << arrshort[j].name << " " << arr.keys(matchinfo[j].seqlist[0]) << " " << matchinfo[j].dist  << " " << matchinfo[j].seqlist.size() << " " << matchinfo[j].cdist << " " << matchinfo[j].seqcluster.size() << endl;
      }
    }
  }else{
    cout << "# validating short sequence placement" << endl;
    for (i=0; i<otuinfo.size(); ++i){
      if (otuinfo[i].seqs.size()>1)
        varr.add(otuinfo[i].seqs[0]);
    }
    matchinfo.init(varr.size());

//    for (k=0; k+winlen<seqlen; k+=20){
    k=300;
    {
      cout << "## k " << k << endl;
      for (i=0; i<ncpus; ++i)
        taskman.addTask(validate_short_task2,evararray(k,k+winlen,matchinfo,(const int&)i,(const int&)ncpus));
      taskman.wait();

      for (j=0; j<varr.size(); ++j){
        if (matchinfo[j].eqcount>0 || otuinfo[matchinfo[j].otu].seqs.size()<=minotusize) {
          cout << "# " << k << " " << arr.keys(varr[j]) << " " << matchinfo[j].dist << " " << otuinfo[matchinfo[j].otu].seqs.size() << " " << arr.keys(otuinfo[matchinfo[j].otu].seqs[0]) << " " << matchinfo[j] << endl;
          continue;
        }
        cout << k << " " << arr.keys(varr[j]) << " " << matchinfo[j].dist << " " << otuinfo[matchinfo[j].otu].seqs.size() << " " << arr.keys(otuinfo[matchinfo[j].otu].seqs[0]) << " " << matchinfo[j] << endl;
      }
    }
/*
    cout << "# validating short sequence placement" << endl;
    for (k=0; k<seqlen-winlen; k+=10){
      earray<ematchinfo> matchinfo;

      for (i=0; i<ncpus; ++i)
        taskman.addTask(calcOTUInfo_task,evararray(k,k+winlen,(const int&)i,(const int&)ncpus));
      taskman.wait();

      matchinfo.init(otuinfo.size());
      for (i=0; i<ncpus; ++i)
        taskman.addTask(match.value(),evararray(k,k+winlen,matchinfo,(const int&)i,(const int&)ncpus));
      taskman.wait();

      eintarray sind(iheapsort(matchinfo));

      int alltp=0;
      int alltn=0;

      int allts=0;
      for (j=matchinfo.size()-1; j>=0; --j){
        if (matchinfo[sind[j]].eqcount>0 || otuinfo[matchinfo[sind[j]].otu].seqs.size()<=minotusize) continue;
        if (matchinfo[sind[j]].otu==sind[j]) ++alltp;
        else if (otuinfo[sind[j]].seqs.size()>1){
//          cout << "# " << matchinfo[sind[j]].dist << " " << otuinfo[sind[j]] << " " << otuinfo[matchinfo[sind[j]].otu] << " " << dist_nogap_short_compressed(arr.values(otuinfo[sind[j]].seqs[0]),arr.values(otuinfo[sind[j]].seqs[1]),k*winlen,k*winlen+2*winlen) << " " << dist_nogap_compressed(arr.values(otuinfo[sind[j]].seqs[0]),arr.values(otuinfo[sind[j]].seqs[1]),seqlen) << " " << clusterData.getMergeDistance(otuinfo[sind[j]].seqs[0],otuinfo[matchinfo[sind[j]].otu].seqs[0]) << " " << matchinfo[sind[j]] << endl;
          ++alltn;
        }else{
          cout << "# " << matchinfo[sind[j]].dist << " " << otuinfo[matchinfo[sind[j]].otu] << " " << dist_nogap_compressed(arr.values(otuinfo[sind[j]].seqs[0]),arr.values(otuinfo[matchinfo[sind[j]].otu].seqs[0]),seqlen) << " " << matchinfo[sind[j]] << endl;
        }
      }
//      cout << "# true positives: " << alltp << endl;
//      cout << "# true negatives: " << alltn << endl;
//      cout << k << " " << float(alltn)/(alltn+alltp) << " " << alltn << " " << alltp << " " << otuinfo.size()-singletons << endl;

      float tdist;
      for (i=matchinfo.size()-1; i>=0; --i){
        for (tdist=matchinfo[sind[i]].dist; i>=0 && matchinfo[sind[i]].dist>=tdist; --i);
        int tp=0;
        int fp=0;
        int fps=0;
        int fsing=0;
        for (j=matchinfo.size()-1; j>i; --j){
          if (otuinfo[sind[j]].seqs.size()==1) ++fsing;

          if (matchinfo[sind[j]].eqcount>0 || otuinfo[matchinfo[sind[j]].otu].seqs.size()<=minotusize) continue;
          if (matchinfo[sind[j]].otu==sind[j]) ++tp;
          else if (otuinfo[sind[j]].seqs.size()>1) ++fp;
          else ++fps;
        }
        cout << k << " " << float(alltn)/(alltn+alltp) << " " << alltn << " " << alltp << " " << otuinfo.size()-singletons << " " << tdist << " " << float(tp)/alltp << " " << float(fp)/alltn << " " << float(fps)/(fps+tp+fp) << " " << float(fsing)/singletons << endl;
      }
    }
*/
  }

  return(0);
}

