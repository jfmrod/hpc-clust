#ifndef CLUSTER_COMMON_H
#define CLUSTER_COMMON_H

#include <eutils/evar.h>
#include <eutils/estr.h>
#include <eutils/ernd.h>
#include <eutils/ebasicarray.h>
#include <eutils/eintarray.h>
#include <eutils/ebasichashmap.h>
#include <eutils/estrhash.h>
#include <eutils/etimer.h>
#include <eutils/eblockarray.h>

#include <list>
#include <math.h>
#include <unistd.h>

#include <eutils/ethread.h>

#include "eseqcluster.h"
#include "eseqclustersingle.h"
#include "eseqclustercount.h"
#include "lookuptables.h"

extern float warnMemThres;
extern bool warnedMemThres;
extern float exitMemThres;
extern bool ignoreMemThres;

/*
extern char lt_gap_count[0x1u << 16];

extern char lt_nogap_count[0x1u << 16];
extern char lt_nogap_len[0x1u << 16];
*/

extern char lt_tamura_p[0x1u << 16];
extern char lt_tamura_q[0x1u << 16];
extern char lt_tamura_len[0x1u << 16];
extern char lt_tamura_gc[0x1u << 16];


//void gap_initLookupTable();
//void nogap_initLookupTable();
//void initLookupTable();

class eshortseq
{
 public:
  int b;
  int e;
  estr seq;
  estr name;

  void compress();
};


inline unsigned char nuc_compress(unsigned char c){
  switch(c){
   case 'A': return(0x00u);
   case 'T': return(0x01u);
   case 'U': return(0x01u);
   case 'G': return(0x02u);
   case 'C': return(0x03u);
   case 'R': return(0x04u);
   case 'Y': return(0x05u);
   case 'S': return(0x06u);
   case 'W': return(0x07u);
   case 'K': return(0x08u);
   case 'M': return(0x09u);
   case 'B': return(0x0Au);
   case 'D': return(0x0Bu);
   case 'H': return(0x0Cu);
   case 'V': return(0x0Du);
   case 'N': return(0x0Eu);
   case '-': return(0x0Fu);
   case '.': return(0x0Fu);
  }
  ldie("unknown nucleotide: "+estr(c));
  return(0x0F);
}

inline void getL2(int& l,int w,int x,int y)
{
  if (y<x) { int t; t=x; x=y; y=t; }

  --w;
  if (x>w/2) { x=w-x; y-=1; l=x*w+y; return; }

  y-=x+1;
  l=x*w+y;
  return;
}

inline void getL(int& l,int w,int x,int y)
{
  --w;
  if (x>w/2) { x=w-x; y-=1; l=x*w+y; return; }

  y-=x+1;
  l=x*w+y;
  return;
}

inline void getXY(int l,int w,int& x,int& y)
{
  --w;
  x=(l/w);
  y=(l%w);
  if (y<w-x){ y+=x+1; return; }
  y+=1;
  x=w-x;
}

inline float dist_nogap(const estr& s1,const estr& s2,int gc1,int gc2)
{
  int i;
  int count=0;
  int gcount=0;
  for (i=0; i<s1._strlen; ++i){
    if (s1._str[i]==s2._str[i]) { if (s1._str[i]=='-') ++gcount; else ++count; }
  }
  return((float)count/(float)(s1._strlen-gc1-gc2+gcount));
}

const long int b4_m0=0x0Fl;
const long int b4_m1=0x0F0l;
const long int b4_m2=0x0F00l;
const long int b4_m3=0x0F000l;
const long int b4_m4=0x0F0000l;
const long int b4_m5=0x0F00000l;
const long int b4_m6=0x0F000000l;
const long int b4_m7=0x0F0000000l;
const long int b4_m8=0x0F00000000l;
const long int b4_m9=0x0F000000000l;
const long int b4_m10=0x0F0000000000l;
const long int b4_m11=0x0F00000000000l;
const long int b4_m12=0x0F000000000000l;
const long int b4_m13=0x0F0000000000000l;
const long int b4_m14=0x0F00000000000000l;
const long int b4_m15=0x0F000000000000000l;
const long int b4_umask=0x1111111111111111;

const long int b8_m0=0x00000000000000FFl;
const long int b8_m1=0x000000000000FF00l;
const long int b8_m2=0x0000000000FF0000l;
const long int b8_m3=0x00000000FF000000l;
const long int b8_m4=0x000000FF00000000l;
const long int b8_m5=0x0000FF0000000000l;
const long int b8_m6=0x00FF000000000000l;
const long int b8_m7=0xFF00000000000000l;

const long int b8_d0='-';
const long int b8_d1=b8_d0 << 8;
const long int b8_d2=b8_d0 << 16;
const long int b8_d3=b8_d0 << 24;
const long int b8_d4=b8_d0 << 32;
const long int b8_d5=b8_d0 << 40;
const long int b8_d6=b8_d0 << 48;
const long int b8_d7=b8_d0 << 56;


inline void dist_inc(long int a1,long int a2,long int mask,int& count){
  if ((a1&mask)==(a2&mask))
    ++count;
}

inline void dist_nogap_inc2(long int a1,long int a2,long int mask,int& count,int& len){
  if ((a1&a2&mask)==mask)
    --len;
  else if ((a1&mask)==(a2&mask))
    ++count;
//  if ((a1&mask)==(a2&mask)){
//    if ((a1&mask)!=mask)
//      ++count;
//    else
//      --len;
//  }
}

inline void dist_nogap_inc3(long int a1,long int a2,long int mask,int& count,int& len){
  if ((a1&a2&mask)==mask)
    ++len;
  if ((a1&mask)==(a2&mask))
    ++count;
//  if ((a1&mask)==(a2&mask)){
//    if ((a1&mask)!=mask)
//      ++count;
//    else
//      --len;
//  }
}

//inline void dist_nogap_inc(long int a1,long int a2,long int mask,int& count,int& len){
//  if (!((a1&mask)==mask || (a2&mask)==mask)){
//    if ((a1&mask)==(a2&mask)) ++count;
//    ++len;
//  }
//}



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

inline void dist_tamura_inc(long int a1,long int a2,long int mask,int& P,int& Q,int& GC1,int& GC2,int& len){
  if ((a1&mask)==mask || (a2&mask)==mask)
    --len;
  else if (((a1^a2)&mask)==((0x02*b4_umask)&mask))
    ++P;
  else if (((a1^a2)&mask)==((0x03*b4_umask)&mask))
    ++Q;
  
  if ((a1&mask)==((0x02*b4_umask)&mask) || (a1&mask)==((0x03*b4_umask)&mask))
    ++GC1;

  if ((a2&mask)==((0x02*b4_umask)&mask) || (a2&mask)==((0x03*b4_umask)&mask))
    ++GC2;
}

inline float dist_tamura_compressed(const estr& s1,const estr& s2,int seqlen)
{
  int len=seqlen;
  int P=0; // transitions
  int Q=0; // transversions
  int GC1=0; // GC content seq1
  int GC2=0; // GC content seq2

  long int *ep1=(long int*)(s1._str)+(s1._strlen/8);
  long int *p1=(long int*)s1._str;
  long int *p2=(long int*)s2._str;
  for (; p1!=ep1; ++p1,++p2){
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

inline float dist_tamura_compressed2(const estr& s1,const estr& s2,int seqlen)
{
  int len=0;
  int P=0; // transitions
  int Q=0; // transversions
  int GC1=0; // GC content seq1
  int GC2=0; // GC content seq2

  unsigned long *ep1=(unsigned long*)(s1._str)+(s1._strlen/8);
  unsigned long *p1=(unsigned long*)s1._str;
  unsigned long *p2=(unsigned long*)s2._str;
  unsigned long tx,ta;
  for (; p1!=ep1; ++p1,++p2){
    tx=((*p1)^(*p2));
    ta=((*p1)&(*p2));
    P+=lt_tamura_p[ tx&0xffffu ] + lt_tamura_p[ (tx>>16)&0xffffu ] + lt_tamura_p[ (tx>>32)&0xffffu ] + lt_tamura_p[ (tx>>48)&0xffffu ];
    Q+=lt_tamura_q[ tx&0xffffu ] + lt_tamura_q[ (tx>>16)&0xffffu ] + lt_tamura_q[ (tx>>32)&0xffffu ] + lt_tamura_q[ (tx>>48)&0xffffu ];
    len-=lt_tamura_len[ ta&0xffffu ] + lt_tamura_len[ (ta>>16)&0xffffu ] + lt_tamura_len[ (ta>>32)&0xffffu ] + lt_tamura_len[ (ta>>48)&0xffffu ];
    GC1+=lt_tamura_gc[ (*p1)&0xffffu ] + lt_tamura_gc[ ((*p1)>>16)&0xffffu ] + lt_tamura_gc[ ((*p1)>>32)&0xffffu ] + lt_tamura_gc[ ((*p1)>>48)&0xffffu ];
    GC2+=lt_tamura_gc[ (*p2)&0xffffu ] + lt_tamura_gc[ ((*p2)>>16)&0xffffu ] + lt_tamura_gc[ ((*p2)>>32)&0xffffu ] + lt_tamura_gc[ ((*p2)>>48)&0xffffu ];
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


/*
void initDistMatrix();

extern int gap_matrix[16][16];

inline float dist_compressed2(const estr& s1,const estr& s2,int seqlen)
{
  int len=seqlen;
  int count=0;
  unsigned long int *ep1=(unsigned long int*)(s1._str)+(s1._strlen/8);
  unsigned long int *p1=(unsigned long int*)s1._str;
  unsigned long int *p2=(unsigned long int*)s2._str;
  unsigned long int tp1,tp2;
  for (; p1!=ep1; ++p1,++p2){
    tp1=*p1,tp2=*p2; count+=gap_matrix[b4_m0&tp1][b4_m0&tp2];
    tp1>>=4,tp2>>=4; count+=gap_matrix[b4_m0&tp1][b4_m0&tp2];
    tp1>>=4,tp2>>=4; count+=gap_matrix[b4_m0&tp1][b4_m0&tp2];
    tp1>>=4,tp2>>=4; count+=gap_matrix[b4_m0&tp1][b4_m0&tp2];
    tp1>>=4,tp2>>=4; count+=gap_matrix[b4_m0&tp1][b4_m0&tp2];
    tp1>>=4,tp2>>=4; count+=gap_matrix[b4_m0&tp1][b4_m0&tp2];
    tp1>>=4,tp2>>=4; count+=gap_matrix[b4_m0&tp1][b4_m0&tp2];
    tp1>>=4,tp2>>=4; count+=gap_matrix[b4_m0&tp1][b4_m0&tp2];
    tp1>>=4,tp2>>=4; count+=gap_matrix[b4_m0&tp1][b4_m0&tp2];
    tp1>>=4,tp2>>=4; count+=gap_matrix[b4_m0&tp1][b4_m0&tp2];
    tp1>>=4,tp2>>=4; count+=gap_matrix[b4_m0&tp1][b4_m0&tp2];
    tp1>>=4,tp2>>=4; count+=gap_matrix[b4_m0&tp1][b4_m0&tp2];
    tp1>>=4,tp2>>=4; count+=gap_matrix[b4_m0&tp1][b4_m0&tp2];
    tp1>>=4,tp2>>=4; count+=gap_matrix[b4_m0&tp1][b4_m0&tp2];
    tp1>>=4,tp2>>=4; count+=gap_matrix[b4_m0&tp1][b4_m0&tp2];
    tp1>>=4,tp2>>=4; count+=gap_matrix[b4_m0&tp1][b4_m0&tp2];
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
  return((float)count/(float)seqlen);
}
*/

/*
inline float dist_compressed(const estr& s1,const estr& s2,int seqlen)
{
  int len=seqlen;
  int count=0;
  long int *ep1=(long int*)(s1._str)+(s1._strlen/8);
  long int *p1=(long int*)s1._str;
  long int *p2=(long int*)s2._str;
  for (; p1!=ep1; ++p1,++p2){
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
  return((float)count/(float)seqlen);
}
*/

inline float dist_compressed2(const estr& s1,const estr& s2,int seqlen)
{
  int len=seqlen;
  int count=0;

  unsigned long *ep1=(unsigned long*)(s1._str)+(s1._strlen/8)-1;
  unsigned long *p1=(unsigned long*)s1._str;
  unsigned long *p2=(unsigned long*)s2._str;
  unsigned long t;
  for (; p1!=ep1; ++p1,++p2){
    t=((*p1) ^ (*p2));
    count+=lt_gap_count[ t&0xfffful ] + lt_gap_count[ (t>>16)&0xfffful ] + lt_gap_count[ (t>>32)&0xfffful ] + lt_gap_count[ (t>>48)&0xfffful ];
  }
  t=((*p1) ^ (*p2));
  count+=lt_gap_count[ t&0xfffful ] + lt_gap_count[ (t>>16)&0xfffful ] + lt_gap_count[ (t>>32)&0xfffful ] + lt_gap_count[ (t>>48)&0xfffful ] - (16-seqlen%16);
  return((float)count/(float)seqlen);
}


inline float dist_compressed(const estr& s1,const estr& s2,int seqlen)
{
  int len=seqlen;
  int count=0;
  long int *ep1=(long int*)(s1._str)+(s1._strlen/8)-1;
  long int *p1=(long int*)s1._str;
  long int *p2=(long int*)s2._str;
  long int tp1,tp2;
  for (; p1!=ep1; ++p1,++p2){
    tp1=*p1,tp2=*p2; dist_inc(tp1,tp2,b4_m0,count);
    tp1>>=4,tp2>>=4; dist_inc(tp1,tp2,b4_m0,count);
    tp1>>=4,tp2>>=4; dist_inc(tp1,tp2,b4_m0,count);
    tp1>>=4,tp2>>=4; dist_inc(tp1,tp2,b4_m0,count);

    tp1>>=4,tp2>>=4; dist_inc(tp1,tp2,b4_m0,count);
    tp1>>=4,tp2>>=4; dist_inc(tp1,tp2,b4_m0,count);
    tp1>>=4,tp2>>=4; dist_inc(tp1,tp2,b4_m0,count);
    tp1>>=4,tp2>>=4; dist_inc(tp1,tp2,b4_m0,count);

    tp1>>=4,tp2>>=4; dist_inc(tp1,tp2,b4_m0,count);
    tp1>>=4,tp2>>=4; dist_inc(tp1,tp2,b4_m0,count);
    tp1>>=4,tp2>>=4; dist_inc(tp1,tp2,b4_m0,count);
    tp1>>=4,tp2>>=4; dist_inc(tp1,tp2,b4_m0,count);

    tp1>>=4,tp2>>=4; dist_inc(tp1,tp2,b4_m0,count);
    tp1>>=4,tp2>>=4; dist_inc(tp1,tp2,b4_m0,count);
    tp1>>=4,tp2>>=4; dist_inc(tp1,tp2,b4_m0,count);
    tp1>>=4,tp2>>=4; dist_inc(tp1,tp2,b4_m0,count);
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
  return((float)count/(float)seqlen);
}

inline void dist_nogap_inc(const estr& s1,const estr& s2,int i,int& count,int& len){
  int bp=i >> 4; // i/16
  int bm=(i%16)*4;
  unsigned long int mask=(b4_m0 << bm);
  
  if ((*(((unsigned long int*)s1._str)+bp)&mask)==mask && (*(((unsigned long int*)s2._str)+bp)&mask)==mask)
    --len;
  else if ((*(((unsigned long int*)s1._str)+bp)&mask)==(*(((unsigned long int*)s2._str)+bp)&mask))
    ++count;
}

inline void dist_nogap_dec(const estr& s1,const estr& s2,int i,int& count,int& len){
  int bp=i >> 4; // i/16
  int bm=(i%16)*4;
  unsigned long int mask=(b4_m0 << bm);
  
  if ((*(((unsigned long int*)s1._str)+bp)&mask)==mask && (*(((unsigned long int*)s2._str)+bp)&mask)==mask)
    ++len;
  else if ((*(((unsigned long int*)s1._str)+bp)&mask)==(*(((unsigned long int*)s2._str)+bp)&mask))
    --count;
}

inline float dist_nogap_compressed_window(const estr& s1,const estr& s2,int seqlen,int winlen)
{
  int len=winlen;
  int count=0;
  int i,j;
  for (i=0; i<seqlen && i<winlen; ++i)
    dist_nogap_inc(s1,s2,i,count,len);

  float tmpdiff=float(count)/len;
  float maxdiff=tmpdiff;
  for (j=0; i<seqlen; ++i,++j){
    dist_nogap_inc(s1,s2,i,count,len);
    dist_nogap_dec(s1,s2,j,count,len);
    tmpdiff=float(count)/len;
    if (tmpdiff<maxdiff) maxdiff=tmpdiff;
  }

  return(maxdiff);
}

inline float dist_nogap_compressed2(const estr& s1,const estr& s2,int seqlen)
{
  int len=0;
  int count=0;
  unsigned long *ep1=(unsigned long*)(s1._str)+(s1._strlen/8);
  unsigned long *p1=(unsigned long*)s1._str;
  unsigned long *p2=(unsigned long*)s2._str;
  unsigned long tx,ta;
  for (; p1!=ep1; ++p1,++p2){
    tx=((*p1) ^ (*p2));
    ta=((*p1) & (*p2));
    count+=lt_nogap_count[ tx&0xffffu ] + lt_nogap_count[ (tx>>16)&0xffffu ] + lt_nogap_count[ (tx>>32)&0xffffu ] + lt_nogap_count[ (tx>>48)&0xffffu ];
    len+=lt_nogap_len[ ta&0xffffu ] + lt_nogap_len[ (ta>>16)&0xffffu ] + lt_nogap_len[ (ta>>32)&0xffffu ] + lt_nogap_len[ (ta>>48)&0xffffu ];
  }

  tx=((*p1) ^ (*p2));
  ta=((*p1) & (*p2));
  count+=lt_nogap_count[ tx&0xffffu ] + lt_nogap_count[ (tx>>16)&0xffffu ] + lt_nogap_count[ (tx>>32)&0xffffu ] + lt_nogap_count[ (tx>>48)&0xffffu ] - (16-seqlen%16);
  len+=lt_nogap_len[ ta&0xffffu ] + lt_nogap_len[ (ta>>16)&0xffffu ] + lt_nogap_len[ (ta>>32)&0xffffu ] + lt_nogap_len[ (ta>>48)&0xffffu ];
  return((float)(count-len)/(float)(seqlen-len));
}

inline float dist_nogap_compressed(const estr& s1,const estr& s2,int seqlen)
{
  int len=seqlen;
  int count=0;
  long int *ep1=(long int*)(s1._str)+(s1._strlen/8);
  long int *p1=(long int*)s1._str;
  long int *p2=(long int*)s2._str;
  for (; p1!=ep1; ++p1,++p2){
    dist_nogap_inc2(*p1,*p2,b4_m0,count,len);
    dist_nogap_inc2(*p1,*p2,b4_m1,count,len);
    dist_nogap_inc2(*p1,*p2,b4_m2,count,len);
    dist_nogap_inc2(*p1,*p2,b4_m3,count,len);
    dist_nogap_inc2(*p1,*p2,b4_m4,count,len);
    dist_nogap_inc2(*p1,*p2,b4_m5,count,len);
    dist_nogap_inc2(*p1,*p2,b4_m6,count,len);
    dist_nogap_inc2(*p1,*p2,b4_m7,count,len);
    dist_nogap_inc2(*p1,*p2,b4_m8,count,len);
    dist_nogap_inc2(*p1,*p2,b4_m9,count,len);
    dist_nogap_inc2(*p1,*p2,b4_m10,count,len);
    dist_nogap_inc2(*p1,*p2,b4_m11,count,len);
    dist_nogap_inc2(*p1,*p2,b4_m12,count,len);
    dist_nogap_inc2(*p1,*p2,b4_m13,count,len);
    dist_nogap_inc2(*p1,*p2,b4_m14,count,len);
    dist_nogap_inc2(*p1,*p2,b4_m15,count,len);
  }

  switch (seqlen%16){
   case 15:
    dist_nogap_inc2(*p1,*p2,b4_m14,count,len);
   case 14:
    dist_nogap_inc2(*p1,*p2,b4_m13,count,len);
   case 13:
    dist_nogap_inc2(*p1,*p2,b4_m12,count,len);
   case 12:
    dist_nogap_inc2(*p1,*p2,b4_m11,count,len);
   case 11:
    dist_nogap_inc2(*p1,*p2,b4_m10,count,len);
   case 10:
    dist_nogap_inc2(*p1,*p2,b4_m9,count,len);
   case 9:
    dist_nogap_inc2(*p1,*p2,b4_m8,count,len);
   case 8:
    dist_nogap_inc2(*p1,*p2,b4_m7,count,len);
   case 7:
    dist_nogap_inc2(*p1,*p2,b4_m6,count,len);
   case 6:
    dist_nogap_inc2(*p1,*p2,b4_m5,count,len);
   case 5:
    dist_nogap_inc2(*p1,*p2,b4_m4,count,len);
   case 4:
    dist_nogap_inc2(*p1,*p2,b4_m3,count,len);
   case 3:
    dist_nogap_inc2(*p1,*p2,b4_m2,count,len);
   case 2:
    dist_nogap_inc2(*p1,*p2,b4_m1,count,len);
   case 1:
    dist_nogap_inc2(*p1,*p2,b4_m0,count,len);
  }
  return((float)count/(float)len);
}


inline float dist_nogapsingle_compressed(const estr& s1,const estr& s2,int seqlen) // count a continuous gap as a single mismatch, gap-gap does not count
{
  int gapmiss=0;
  int len=seqlen;
  int count=0;
  long int *ep1=(long int*)(s1._str)+MIN(s1._strlen/8,seqlen/16);
  long int *p1=(long int*)(s1._str);
  long int *p2=(long int*)(s2._str);
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





inline void dist_nogap_inc_dash(long int a1,long int a2,long int mask,long int dash,int& count,int& len){
  if ((a1&mask)==(a2&mask)){
    if ((a1&mask)!=mask)
      ++count;
    else
      --len;
  }
}

//inline void dist_nogap_inc_dash(long int a1,long int a2,long int mask,long int dash,int& count,int& len){
//  if (!((a1&mask)==dash || (a2&mask)==dash)){
//    if ((a1&mask)==(a2&mask)) ++count;
//    ++len;
//  }
//}

inline float dist_nogap(const estr& s1,const estr& s2)
{
  int count=0;
  int len=s1._strlen;
  long int *ep1=((long int*)s1._str)+(s1._strlen/8);
  long int *p1=(long int*)s1._str;
  long int *p2=(long int*)s2._str;
  for (; p1!=ep1; ++p1,++p2){
    dist_nogap_inc_dash(*p1,*p2,b8_m0,b8_d0,count,len);
    dist_nogap_inc_dash(*p1,*p2,b8_m1,b8_d1,count,len);
    dist_nogap_inc_dash(*p1,*p2,b8_m2,b8_d2,count,len);
    dist_nogap_inc_dash(*p1,*p2,b8_m3,b8_d3,count,len);
    dist_nogap_inc_dash(*p1,*p2,b8_m4,b8_d4,count,len);
    dist_nogap_inc_dash(*p1,*p2,b8_m5,b8_d5,count,len);
    dist_nogap_inc_dash(*p1,*p2,b8_m6,b8_d6,count,len);
    dist_nogap_inc_dash(*p1,*p2,b8_m7,b8_d7,count,len);
  }

  switch (s1._strlen%8){
   case 7:
    dist_nogap_inc_dash(*p1,*p2,b8_m6,b8_d6,count,len);
   case 6:
    dist_nogap_inc_dash(*p1,*p2,b8_m5,b8_d5,count,len);
   case 5:
    dist_nogap_inc_dash(*p1,*p2,b8_m4,b8_d4,count,len);
   case 4:
    dist_nogap_inc_dash(*p1,*p2,b8_m3,b8_d3,count,len);
   case 3:
    dist_nogap_inc_dash(*p1,*p2,b8_m2,b8_d2,count,len);
   case 2:
    dist_nogap_inc_dash(*p1,*p2,b8_m1,b8_d1,count,len);
   case 1:
    dist_nogap_inc_dash(*p1,*p2,b8_m0,b8_d0,count,len);
  }
  return((float)count/(float)len);
}

/*
inline float dist_nogap(const estr& s1,const estr& s2)
{
  int i;
  int count=0;
  int len=0;
  for (i=0; i<s1._strlen; ++i){
    if (s1._str[i]=='-' || s2._str[i]=='-') continue;
    if (s1._str[i]==s2._str[i]) ++count;
    ++len;
  }
  return((float)count/(float)len);
}
*/

inline float dist(const estr& s1,const estr& s2)
{
  int i;
  int count=0;
  for (i=0; i<s1.len(); ++i){
    if (s1._str[i]==s2._str[i]) ++count;
  }
  return((float)count/(float)s1.len());
}

inline float cooc_dist(int i,int j,earray<eintarray>& neigharr)
{
  int k;

  int cooc1,cooc2,common;

  cooc1=0; cooc2=0; common=0;
  if (neigharr[i].size()==0 || neigharr[j].size()==0) return(2.0);
  ldieif(j>=neigharr.size(),"j larger than neigharr.size()? "+estr(j)+">"+estr(neigharr.size()));
  ldieif(i>=neigharr.size(),"i larger than neigharr.size()? "+estr(i)+">"+estr(neigharr.size()));
  ldieif(neigharr[i].size() != neigharr[j].size(),"size mismatch: "+estr(neigharr[i].size())+" != "+estr(neigharr[j].size()));

  for (k=0; k<neigharr[i].size(); ++k){
    if (neigharr[i][k]!=0){ ++cooc1; if (neigharr[j][k]!=0) ++common; }
    if (neigharr[j][k]!=0){ ++cooc2; }
  }
  return(1.0-(float)common/(cooc1+cooc2-common));
}

inline void xy2estr(int x,int y,estr& str)
{
  str.clear();
  if (x<y){
    serialint(x,str);
    serialint(y,str);
  }else{
    serialint(y,str);
    serialint(x,str);
  }
}

/*
template <class T,class M,class K,float (*fdist)(const estr&,const estr&,int)>
int t_calc_dists_noise(emutex& mutex,T& arr,K& dists,int seqlen,int node,int tnodes,float thres)
{
  ernd rng;
  float noise=0.0125;
  long int i,i2,j;
  long int start,end;

  start=(long int)(node)*(long int)(arr.size()-1)/(long int)(2*tnodes);
  end=(long int)(node+1)*(long int)(arr.size()-1)/(long int)(2*tnodes);

  float tmpid,tmpid2,tmpid3;
  K tmpdists;

  for (i=start; i<end; ++i){
    for (j=i+1; j<arr.size(); ++j){
      tmpid=fdist(arr[i],arr[j],seqlen)+rng.gaussian(noise);
      if (tmpid>1.0) tmpid=1.0; else if (tmpid<0.0) tmpid=0.0;
      if (tmpid>=thres) tmpdists.add(M(i,j,tmpid));
    }
    i2=arr.size()-i-2;
    for (j=i2+1; j<arr.size(); ++j){
      tmpid=fdist(arr[i2],arr[j],seqlen)+rng.gaussian(noise);
      if (tmpid>1.0) tmpid=1.0; else if (tmpid<0.0) tmpid=0.0;
      if (tmpid>=thres) tmpdists.add(M(i2,j,tmpid));
    }
  }
  if (node==tnodes-1 && arr.size()%2==0){
    i=arr.size()/2-1;
    for (j=i+1; j<arr.size(); ++j){
      tmpid=fdist(arr[i],arr[j],seqlen)+rng.gaussian(noise);
      if (tmpid>1.0) tmpid=1.0; else if (tmpid<0.0) tmpid=0.0;
      if (tmpid>=thres) tmpdists.add(M(i,j,tmpid));
    }
  }
  mutex.lock();
  dists+=tmpdists;
  mutex.unlock();
  return(tmpdists.size());
}
*/

template <class T,class M,class K,float (*fdist)(const eshortseq&,const eshortseq&)>
int t_calc_dists(emutex& mutex,T& arr,K& dists,int node,int tnodes,float thres)
{
  long int i,i2,j;
  long int start,end;

  start=(long int)(node)*(long int)(arr.size()-1)/(long int)(2*tnodes);
  end=(long int)(node+1)*(long int)(arr.size()-1)/(long int)(2*tnodes);

  float tmpid,tmpid2,tmpid3;
  K tmpdists;

  for (i=start; i<end; ++i){
    for (j=i+1; j<arr.size(); ++j){
      tmpid=fdist(arr[i],arr[j]);
      if (tmpid>=thres) tmpdists.add(M(i,j,tmpid));
    }
    i2=arr.size()-i-2;
    for (j=i2+1; j<arr.size(); ++j){
      tmpid=fdist(arr[i2],arr[j]);
      if (tmpid>=thres) tmpdists.add(M(i2,j,tmpid));
    }
  }
  if (node==tnodes-1 && arr.size()%2==0){
    i=arr.size()/2-1;
    for (j=i+1; j<arr.size(); ++j){
      tmpid=fdist(arr[i],arr[j]);
      if (tmpid>=thres) tmpdists.add(M(i,j,tmpid));
    }
  }
  mutex.lock();
  dists+=tmpdists;
  mutex.unlock();
  return(0);
}

template <class T,class M,float (*fdist)(const estr&,const estr&,int)>
int part_calc_dists_u(emutex& mutex,ebasicarray<INDTYPE>& uniqind,T& arr,ebasicarray<INDTYPE>& curotuid,int seqlen,long int node,long int tnodes,float thres)
{
  long int i,i2,j;
  long int start,end;

  mutex.lock();
  ebasicarray<INDTYPE> otuid(curotuid);
  mutex.unlock();

  start=((long int)(node)*(long int)(uniqind.size()-1l))/(long int)(2l*tnodes);
  end=((long int)(node+1l)*(long int)(uniqind.size()-1l))/(long int)(2l*tnodes);

//  cout << "start: " << start << " end: " << end << " uniqind.size(): " << uniqind.size() << endl;

  float tmpid,tmpid2,tmpid3;

  long tmpoid,nextoid;
  long oid_i,oid_j;
  for (i=start; i<end; ++i){
    oid_i=otuid[i];
    if (oid_i!=otuid[oid_i]){
      for (oid_i=otuid[oid_i]; oid_i!=otuid[oid_i]; oid_i=otuid[oid_i]);
      for (tmpoid=i; tmpoid!=otuid[tmpoid]; tmpoid=nextoid) { nextoid=otuid[tmpoid]; otuid[tmpoid]=oid_i; }
    }
    for (j=i+1; j<uniqind.size(); ++j){
      oid_j=otuid[j];
      if (oid_i==oid_j) continue; // skip distances for sequences already in the same otu
      if (oid_j!=otuid[oid_j]){
        for (oid_j=otuid[oid_j]; oid_j!=otuid[oid_j]; oid_j=otuid[oid_j]);
        for (tmpoid=j; tmpoid!=otuid[tmpoid]; tmpoid=nextoid) { nextoid=otuid[tmpoid]; otuid[tmpoid]=oid_j; }
      }
      if (oid_i==oid_j) continue; // skip distances for sequences already in the same otu
      tmpid=fdist(arr[uniqind[i]],arr[uniqind[j]],seqlen);
      if (tmpid>=thres){
        otuid[oid_j]=oid_i;
//        cout << "merge: " << oid_i << " " << oid_j << endl;
      }
    }
    i2=uniqind.size()-i-2l;
    oid_i=otuid[i2];
    if (oid_i!=otuid[oid_i]){
      for (oid_i=otuid[oid_i]; oid_i!=otuid[oid_i]; oid_i=otuid[oid_i]);
      for (tmpoid=i2; tmpoid!=otuid[tmpoid]; tmpoid=nextoid) { nextoid=otuid[tmpoid]; otuid[tmpoid]=oid_i; }
    }
    for (j=i2+1l; j<uniqind.size(); ++j){
      oid_j=otuid[j];
      if (oid_i==oid_j) continue; // skip distances for sequences already in the same otu
      if (oid_j!=otuid[oid_j]){
        for (oid_j=otuid[oid_j]; oid_j!=otuid[oid_j]; oid_j=otuid[oid_j]);
        for (tmpoid=j; tmpoid!=otuid[tmpoid]; tmpoid=nextoid) { nextoid=otuid[tmpoid]; otuid[tmpoid]=oid_j; }
      }
      if (oid_i==oid_j) continue; // skip distances for sequences already in the same otu
      tmpid=fdist(arr[uniqind[i2]],arr[uniqind[j]],seqlen);
      if (tmpid>=thres){
        otuid[oid_j]=oid_i;
//        cout << "merge: " << oid_i << " " << oid_j << endl;
      }
    }
  }
  if (node==tnodes-1l && uniqind.size()%2==0){
    i=uniqind.size()/2-1l;
    oid_i=otuid[i];
    if (oid_i!=otuid[oid_i]){
      for (oid_i=otuid[oid_i]; oid_i!=otuid[oid_i]; oid_i=otuid[oid_i]);
      for (tmpoid=i; tmpoid!=otuid[tmpoid]; tmpoid=nextoid) { nextoid=otuid[tmpoid]; otuid[tmpoid]=oid_i; }
    }
    for (j=i+1l; j<uniqind.size(); ++j){
      oid_j=otuid[j];
      if (oid_i==oid_j) continue; // skip distances for sequences already in the same otu
      if (oid_j!=otuid[oid_j]){
        for (oid_j=otuid[oid_j]; oid_j!=otuid[oid_j]; oid_j=otuid[oid_j]);
        for (tmpoid=j; tmpoid!=otuid[tmpoid]; tmpoid=nextoid) { nextoid=otuid[tmpoid]; otuid[tmpoid]=oid_j; }
      }
      if (oid_i==oid_j) continue; // skip distances for sequences already in the same otu
      tmpid=fdist(arr[uniqind[i]],arr[uniqind[j]],seqlen);
      if (tmpid>=thres){
        otuid[oid_j]=oid_i;
//        cout << "merge: " << oid_i << " " << oid_j << endl;
      }
    }
  }
  // update list of membership
  long notucount=0l;
  for (long i=0; i<otuid.size(); ++i){
    oid_i=otuid[i];
    if (i==oid_i){ ++notucount; continue; }
    for (oid_i=otuid[oid_i]; oid_i!=otuid[oid_i]; oid_i=otuid[oid_i]);
    for (tmpoid=i; tmpoid!=otuid[tmpoid]; tmpoid=nextoid) { nextoid=otuid[tmpoid]; otuid[tmpoid]=oid_i; }
  }
  mutex.lock();
  long noid;
  for (long i=0; i<otuid.size(); ++i){
    if (otuid[i]==curotuid[i] || i==otuid[i]) continue; //unchanged otuid
    // do not change otuid's, but change final oid of curotuid 
//    cout << "difference found: " << i << " " << otuid[i] << " " << curotuid[i] << endl;

    for (oid_i=curotuid[i]; oid_i!=curotuid[oid_i]; oid_i=curotuid[oid_i]);

    noid=otuid[i];
    do {
      for (oid_j=curotuid[noid]; oid_j!=curotuid[oid_j]; oid_j=curotuid[oid_j]);
      if (oid_j==oid_i) break;
//      cout << "setting: " << oid_j << " to: " << oid_i << endl;
      curotuid[oid_j]=oid_i;
      noid=oid_j;
    } while (1);
  }
  // update all otu ids
  long otucount=0;
  for (long i=0; i<curotuid.size(); ++i){
    oid_i=curotuid[i];
    if (i==oid_i) { ++otucount; continue; }
    for (oid_i=curotuid[oid_i]; oid_i!=curotuid[oid_i]; oid_i=curotuid[oid_i]);
    for (tmpoid=i; tmpoid!=curotuid[tmpoid]; tmpoid=nextoid) { nextoid=curotuid[tmpoid]; curotuid[tmpoid]=oid_i; }
  }
  if (isatty(2))
    fprintf(stderr,"\r# finished: %.1f%% %li %li %li %li",float(node+1)*100.0/tnodes,otucount,notucount,otuid.size(),curotuid.size());
  mutex.unlock();
  return(0);
}

template <class MT,class M,class CDIST,float (*fdist)(const estr&,const estr&,int)>
//,class M,class K,float (*fdist)(const estr&,const estr&,int)>
int newt_calc_dists_u(MT& mtdata)
//emutex& mutex,ebasicarray<INDTYPE>& uniqind,T& arr,K& dists,int seqlen,long int node,long int tnodes,float thres)
{
  long int i,i2,j;
  long int start,end;
//  cout << "newt_calc_dists" << endl;

  mtdata.m.lock();
  long int node=mtdata.taskCurrent;
  long int tnodes=mtdata.taskTotal;
  if (node==tnodes) { mtdata.m.unlock(); return(0); }
  ++mtdata.taskCurrent;
//  cout << mtdata.taskCurrent << " " << mtdata.uniqind.size() << endl;
  mtdata.m.unlock();
  

  start=((long int)(node)*(long int)(mtdata.uniqind.size()-1l))/(long int)(2l*tnodes);
  end=((long int)(node+1l)*(long int)(mtdata.uniqind.size()-1l))/(long int)(2l*tnodes);

//  cout << "start: " << start << " end: " << end << " uniqind.size(): " << uniqind.size() << endl;

  float tmpid,tmpid2,tmpid3;
  CDIST tmpdists;

  for (i=start; i<end; ++i){
    for (j=i+1; j<mtdata.uniqind.size(); ++j){
      tmpid=fdist(mtdata.arr[mtdata.uniqind[i]],mtdata.arr[mtdata.uniqind[j]],mtdata.seqlen);
      if (tmpid>=mtdata.thres) tmpdists.add(M(mtdata.uniqind[i],mtdata.uniqind[j],tmpid));
    }
    i2=mtdata.uniqind.size()-i-2l;
    for (j=i2+1l; j<mtdata.uniqind.size(); ++j){
      tmpid=fdist(mtdata.arr[mtdata.uniqind[i2]],mtdata.arr[mtdata.uniqind[j]],mtdata.seqlen);
      if (tmpid>=mtdata.thres) tmpdists.add(M(mtdata.uniqind[i2],mtdata.uniqind[j],tmpid));
    }
  }
  if (node==tnodes-1l && mtdata.uniqind.size()%2==0){
    i=mtdata.uniqind.size()/2-1l;
    for (j=i+1l; j<mtdata.uniqind.size(); ++j){
      tmpid=fdist(mtdata.arr[mtdata.uniqind[i]],mtdata.arr[mtdata.uniqind[j]],mtdata.seqlen);
      if (tmpid>=mtdata.thres) tmpdists.add(M(mtdata.uniqind[i],mtdata.uniqind[j],tmpid));
    }
  }
  mtdata.m.lock();
  mtdata.dists+=tmpdists;
  float memUsed=mtdata.dists.size()*sizeof(eseqdist)/1024/1024;
  if (isatty(2))
    fprintf(stderr,"\r# finished: %.1f%% dists: %li (%liMb)",float(node+1)*100.0/tnodes,(long)mtdata.dists.size(),(long)memUsed);
  if (!warnedMemThres && memUsed >= warnMemThres){
    warnedMemThres=true;
    cout << "# WARNING: Exceeded warnMemThres ("<< warnMemThres <<"Mb) with " << mtdata.dists.size() << " ("<< memUsed <<"Mb) stored distance pairs" << endl;
    cout << "#          hpc-clust will exit once it reaches the exitMemThres ("<< exitMemThres <<"Mb)" << endl;
    cout << "#          in this event, increase the clustering threshold or provide fewer sequences for clustering." << endl;
    cout << "#          Please refer to the documentation for further information and tips on improving the memory usage of hpc-clust." << endl;
  }
  if (memUsed >= exitMemThres && !ignoreMemThres){
    cout << "# ERROR:   Exceeded exitMemThres ("<< exitMemThres <<"Mb) with " << mtdata.dists.size() << " ("<< memUsed <<"Mb) stored distance pairs" << endl;
    cout << "#          Increase the clustering threshold or provide fewer sequences for clustering. You can force the program to ignore this threshold" << endl;
    cout << "#          and continue running with the argume: -ignoreMemThres true. Note that your system may become unresponsive or even crash if it runs out of memory" << endl;
    cout << "#          Please refer to the documentation for further information and tips on improving the memory usage of hpc-clust." << endl;
    exit(-1);
  }
  mtdata.m.unlock();
  return(0);
}


template <class T,class M,class K,float (*fdist)(const estr&,const estr&,int)>
int t_calc_dists_u(emutex& mutex,ebasicarray<INDTYPE>& uniqind,T& arr,K& dists,int seqlen,long int node,long int tnodes,float thres)
{
  long int i,i2,j;
  long int start,end;

  start=((long int)(node)*(long int)(uniqind.size()-1l))/(long int)(2l*tnodes);
  end=((long int)(node+1l)*(long int)(uniqind.size()-1l))/(long int)(2l*tnodes);

//  cout << "start: " << start << " end: " << end << " uniqind.size(): " << uniqind.size() << endl;

  float tmpid,tmpid2,tmpid3;
  K tmpdists;

  for (i=start; i<end; ++i){
    for (j=i+1; j<uniqind.size(); ++j){
      tmpid=fdist(arr[uniqind[i]],arr[uniqind[j]],seqlen);
      if (tmpid>=thres) tmpdists.add(M(uniqind[i],uniqind[j],tmpid));
    }
    i2=uniqind.size()-i-2l;
    for (j=i2+1l; j<uniqind.size(); ++j){
      tmpid=fdist(arr[uniqind[i2]],arr[uniqind[j]],seqlen);
      if (tmpid>=thres) tmpdists.add(M(uniqind[i2],uniqind[j],tmpid));
    }
  }
  if (node==tnodes-1l && uniqind.size()%2==0){
    i=uniqind.size()/2-1l;
    for (j=i+1l; j<uniqind.size(); ++j){
      tmpid=fdist(arr[uniqind[i]],arr[uniqind[j]],seqlen);
      if (tmpid>=thres) tmpdists.add(M(uniqind[i],uniqind[j],tmpid));
    }
  }
  mutex.lock();
  dists+=tmpdists;
  float memUsed=dists.size()*sizeof(eseqdist)/1024/1024;
  if (isatty(2))
    fprintf(stderr,"\r# finished: %.1f%% dists: %li (%liMb)",float(node+1)*100.0/tnodes,(long)dists.size(),(long)memUsed);
  if (!warnedMemThres && memUsed >= warnMemThres){
    warnedMemThres=true;
    cout << "# WARNING: Exceeded warnMemThres ("<< warnMemThres <<"Mb) with " << dists.size() << " ("<< memUsed <<"Mb) stored distance pairs" << endl;
    cout << "#          hpc-clust will exit once it reaches the exitMemThres ("<< exitMemThres <<"Mb)" << endl;
    cout << "#          in this event, increase the clustering threshold or provide fewer sequences for clustering." << endl;
    cout << "#          Please refer to the documentation for further information and tips on improving the memory usage of hpc-clust." << endl;
  }
  if (memUsed >= exitMemThres && !ignoreMemThres){
    cout << "# ERROR:   Exceeded exitMemThres ("<< exitMemThres <<"Mb) with " << dists.size() << " ("<< memUsed <<"Mb) stored distance pairs" << endl;
    cout << "#          Increase the clustering threshold or provide fewer sequences for clustering. You can force the program to ignore this threshold" << endl;
    cout << "#          and continue running with the argume: -ignoreMemThres true. Note that your system may become unresponsive or even crash if it runs out of memory" << endl;
    cout << "#          Please refer to the documentation for further information and tips on improving the memory usage of hpc-clust." << endl;
    exit(-1);
  }
  mutex.unlock();
  return(0);
}

template <class T,class M,class K,float (*fdist)(const estr&,const estr&,int)>
int t_calc_dists(emutex& mutex,T& arr,K& dists,int seqlen,int node,int tnodes,float thres)
{
  long int i,i2,j;
  long int start,end;

  start=(long int)(node)*(long int)(arr.size()-1)/(long int)(2*tnodes);
  end=(long int)(node+1)*(long int)(arr.size()-1)/(long int)(2*tnodes);

  float tmpid,tmpid2,tmpid3;
  K tmpdists;

  for (i=start; i<end; ++i){
    for (j=i+1; j<arr.size(); ++j){
      tmpid=fdist(arr[i],arr[j],seqlen);
      if (tmpid>=thres) tmpdists.add(M(i,j,tmpid));
    }
    i2=arr.size()-i-2;
    for (j=i2+1; j<arr.size(); ++j){
      tmpid=fdist(arr[i2],arr[j],seqlen);
      if (tmpid>=thres) tmpdists.add(M(i2,j,tmpid));
    }
  }
  if (node==tnodes-1 && arr.size()%2==0){
    i=arr.size()/2-1;
    for (j=i+1; j<arr.size(); ++j){
      tmpid=fdist(arr[i],arr[j],seqlen);
      if (tmpid>=thres) tmpdists.add(M(i,j,tmpid));
    }
  }
  mutex.lock();
  dists+=tmpdists;
  mutex.unlock();
  return(0);
}

template <class T,class M,class K,float (*fdist)(const estr&,const estr&,int,int)>
int t_calc_dists_window(emutex& mutex,T& arr,K& dists,int seqlen,int node,int tnodes,float thres,int winlen)
{
  long int i,i2,j;
  long int start,end;

  start=(long int)(node)*(long int)(arr.size()-1)/(long int)(2*tnodes);
  end=(long int)(node+1)*(long int)(arr.size()-1)/(long int)(2*tnodes);

  float tmpid,tmpid2,tmpid3;
  K tmpdists;

  for (i=start; i<end; ++i){
    for (j=i+1; j<arr.size(); ++j){
      tmpid=fdist(arr[i],arr[j],seqlen,winlen);
      if (tmpid>=thres) tmpdists.add(M(i,j,tmpid));
    }
    i2=arr.size()-i-2;
    for (j=i2+1; j<arr.size(); ++j){
      tmpid=fdist(arr[i2],arr[j],seqlen,winlen);
      if (tmpid>=thres) tmpdists.add(M(i2,j,tmpid));
    }
  }
  if (node==tnodes-1 && arr.size()%2==0){
    i=arr.size()/2-1;
    for (j=i+1; j<arr.size(); ++j){
      tmpid=fdist(arr[i],arr[j],seqlen,winlen);
      if (tmpid>=thres) tmpdists.add(M(i,j,tmpid));
    }
  }
  mutex.lock();
  dists+=tmpdists;
  mutex.unlock();
  return(0);
}

/*
template <class T,class M,class K,float (*fdist)(const estr&,const estr&,int)>
void t_calc_dists_noise2(T& arr,K& tmpdists,int seqlen,int node,int tnodes,float thres)
{
  float noise=0.025;
  long int i,i2,j;
  long int start,end;
  ernd rng;

  start=(long int)(node)*(long int)(arr.size()-1)/(long int)(2*tnodes);
  end=(long int)(node+1)*(long int)(arr.size()-1)/(long int)(2*tnodes);

  float tmpid,tmpid2,tmpid3;

  for (i=start; i<end; ++i){
    for (j=i+1; j<arr.size(); ++j){
      tmpid=fdist(arr[i],arr[j],seqlen)+rng.gaussian(noise);
      if (tmpid>=thres) { if (tmpid>1.0) tmpid=1.0; tmpdists.add(M(i,j,tmpid)); }
    }
    i2=arr.size()-i-2;
    for (j=i2+1; j<arr.size(); ++j){
      tmpid=fdist(arr[i2],arr[j],seqlen)+rng.gaussian(noise);
      if (tmpid>=thres) { if (tmpid>1.0) tmpid=1.0; tmpdists.add(M(i2,j,tmpid)); }
    }
  }
  if (node==tnodes-1 && arr.size()%2==0){
    i=arr.size()/2-1;
    for (j=i+1; j<arr.size(); ++j){
      tmpid=fdist(arr[i],arr[j],seqlen)+rng.gaussian(noise);
      if (tmpid>=thres) { if (tmpid>1.0) tmpid=1.0; tmpdists.add(M(i,j,tmpid)); }
    }
  }
}

template <class T,class M,class K,float (*fdist)(const estr&,const estr&,int)>
void t_calc_dists2(T& arr,K& tmpdists,int seqlen,int node,int tnodes,float thres)
{
  long int i,i2,j;
  long int start,end;

  start=(long int)(node)*(long int)(arr.size()-1)/(long int)(2*tnodes);
  end=(long int)(node+1)*(long int)(arr.size()-1)/(long int)(2*tnodes);

  float tmpid,tmpid2,tmpid3;

  for (i=start; i<end; ++i){
    for (j=i+1; j<arr.size(); ++j){
      tmpid=fdist(arr[i],arr[j],seqlen);
      if (tmpid>=thres) tmpdists.add(M(i,j,tmpid));
    }
    i2=arr.size()-i-2;
    for (j=i2+1; j<arr.size(); ++j){
      tmpid=fdist(arr[i2],arr[j],seqlen);
      if (tmpid>=thres) tmpdists.add(M(i2,j,tmpid));
    }
  }
  if (node==tnodes-1 && arr.size()%2==0){
    i=arr.size()/2-1;
    for (j=i+1; j<arr.size(); ++j){
      tmpid=fdist(arr[i],arr[j],seqlen);
      if (tmpid>=thres) tmpdists.add(M(i,j,tmpid));
    }
  }
}

*/

//int calc_dists_nogap(estrarray& arr,ebasicarray<eseqdist>& dists,int node,int tnodes,float thres);
//int calc_dists_nogap(estrarray& arr,earray<eseqdist>& dists,int node,int tnodes,float thres);
//int calc_dists_nogap(estrarray& arr,eintarray& arrgaps,earray<eseqdist>& dists,int node,int tnodes,float thres);

//int calc_dists_local(estrarray& arr,ebasicarray<float>& dists,float thres);

void cluster_init(earray<eintarray>& cvec,const estrhashof<int>& arrind,const eintarray& otuarr,int otucount);

void cooc_init(earray<eintarray>& neigharr,const estrarray& arr,const eintarray& otuarr,estrhashof<eintarray>& samples,estrhash& accsample,estrhashof<int>& arrind,int otucount);
void cooc_calc(int start,int end,ebasicarray<float>& dist_mat,earray<eintarray>& neigharr);
void cooc_calc2(int start,int end,ebasicarray<float>& dist_mat,earray<eintarray>& neigharr);

//void load_accs(const estr& filename,estrhash& arr);
//void load_seqs(const estr& filename,estrhash& arr);
void load_accs(const estr& filename,estrarray& arr);
void load_seqs(const estr& filename,estrarray& arr);


//void load_seqs_mutate_compressed(const estr& filename,estrarray& arr,int& seqlen,float avgmutseq);

void load_short_compressed(const estr& filename,estrarray& arr,int& seqlen);
void load_short_compressed(const estr& filename,ebasicarray<eshortseq>& arr);

void load_seqs_compressed(const estr& filename,earray<estr>& arr,int& seqlen);
void load_seqs_compressed(const estr& filename,estrarray& arr,int& seqlen);
void load_seqs_compressed(const estr& filename,estrarray& arr,estrhashof<int>& arrind,int& seqlen);
void load_seqs_compressed(const estr& filename,estrarray& arr,estrhashof<long>& arrind,int& seqlen);
void load_seqs(const estr& filename,estrarray& arr,eintarray& arrgaps);
void load_seqs(const estr& filename,estrhash& arr);
void load_seqs(const estr& filename,estrhashof<int>& arrind);
void load_seqs(const estr& filename,estrarray& arr,estrhashof<int>& arrind);

void load_samples(const estr& filename,estrhashof<int>& arrind,estrhashof<eintarray>& samples,estrhash& accsample);
//void load_samples(const estr& filename,estrhashof<earray<estr> >& samples,estrhash& accsample);

void load_clusters(const estr& filename,estrhashof<int>& arrind,eintarray& otuarr,int& otucount);
void load_clusters(const estr& filename,estrhashof<int>& arrind,earray<eintarray>& otus);

//void cluster_complete(const estrhash& arr,const ebasicarray<float>& dist_mat,const eintarray& dsort,earray<eintarray>& otus,eintarray& otuarr,float t);
//void cluster_single(const estrhash& arr,const ebasicarray<float>& dist_mat,const eintarray& dsort,earray<eintarray>& otus,float t);


#endif /* CLUSTER_COMMON_H */

