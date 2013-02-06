#include <eutils/emain.h>
#include <eutils/efile.h>
#include <eutils/eheap.h>
#include <eutils/etime.h>
#include <eutils/etimer.h>
#include <eutils/eoption.h>
#include <eutils/ethread.h>
#include <eutils/eregexp.h>
#include <eutils/sci/estats.h>

#include "cluster-common.h"

estrarray arr;
estrarray arrshort;

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

inline unsigned char getnuci(const estr& s1,int i)
{
  unsigned long int *p1=(long unsigned int*)(s1._str)+(i/16);
  unsigned long m=0x0ful&((*p1)>>(i%16)*4);
  if (m<=0x03ul) return(m);
  if (m==0xful) return(0x4u);
  return(0x5u);
}

inline char getnuc(const estr& s1,int s)
{
  unsigned long int *p1=(long unsigned int*)(s1._str)+(s/16);
  unsigned long m=(0x0ful<< (s%16)*4);
  switch( 0x0f&((*p1)>>(s%16)*4) ){
   case 0x00: return('A');
   case 0x01: return('T');
   case 0x02: return('G');
   case 0x03: return('C');
   case 0x04: return('R');
   case 0x05: return('Y');
   case 0x06: return('S');
   case 0x07: return('W');
   case 0x08: return('K');
   case 0x09: return('M');
   case 0x0A: return('B');
   case 0x0B: return('D');
   case 0x0C: return('H');
   case 0x0D: return('V');
   case 0x0E: return('N');
   case 0x0F: return('-');
  }
  return('E');
}

inline bool dist_nogap_short_compressed_single(const estr& s1,const estr& s2,int s)
{
  unsigned long int *p1=(long unsigned int*)(s1._str)+(s/16);
  unsigned long int *p2=(long unsigned int*)(s2._str)+(s/16);
  unsigned long m=(0x0ful<< (s%16)*4);
  if (((*p1)&(*p2)&m) == m) return(false);
  if (((*p1)&m)==((*p2)&m)) return(true);
  return(false);
}

inline float dist_nogap_short_compressed2(const estr& s1,const estr& s2,int s,int seqlen)
{
  int len=0;
  int count=0;
  unsigned long int *ep1=(long unsigned int*)(s1._str)+MIN(s1._strlen/8,seqlen/16);
  unsigned long int *p1=(long unsigned int*)(s1._str)+(s/16);
  unsigned long int *p2=(long unsigned int*)(s2._str)+(s/16);
  unsigned long tx,ta,m;
  
  if (p1<ep1 && s%16){
    m=(0xfffffffffffffffful << (s%16)*4);
    tx=((*p1) ^ (*p2))&m;
    ta=((*p1) & (*p2))&m;
    count+=lt_nogap_count[ tx&0xffffu ] + lt_nogap_count[ (tx>>16)&0xffffu ] + lt_nogap_count[ (tx>>32)&0xffffu ] + lt_nogap_count[ (tx>>48)&0xffffu ] - (16-s%16);
    len+=lt_nogap_len[ ta&0xffffu ] + lt_nogap_len[ (ta>>16)&0xffffu ] + lt_nogap_len[ (ta>>32)&0xffffu ] + lt_nogap_len[ (ta>>48)&0xffffu ];
    ++p1; ++p2;
  }

  for (; p1<ep1; ++p1,++p2){
    tx=((*p1) ^ (*p2));
    ta=((*p1) & (*p2));
    count+=lt_nogap_count[ tx&0xffffu ] + lt_nogap_count[ (tx>>16)&0xffffu ] + lt_nogap_count[ (tx>>32)&0xffffu ] + lt_nogap_count[ (tx>>48)&0xffffu ];
    len+=lt_nogap_len[ ta&0xffffu ] + lt_nogap_len[ (ta>>16)&0xffffu ] + lt_nogap_len[ (ta>>32)&0xffffu ] + lt_nogap_len[ (ta>>48)&0xffffu ];
  }

  m=~(0xfffffffffffffffful << (seqlen%16)*4);
  tx=((*p1) ^ (*p2))&m;
  ta=((*p1) & (*p2))&m;
  count+=lt_nogap_count[ tx&0xffffu ] + lt_nogap_count[ (tx>>16)&0xffffu ] + lt_nogap_count[ (tx>>32)&0xffffu ] + lt_nogap_count[ (tx>>48)&0xffffu ] - (16-seqlen%16);
  len+=lt_nogap_len[ ta&0xffffu ] + lt_nogap_len[ (ta>>16)&0xffffu ] + lt_nogap_len[ (ta>>32)&0xffffu ] + lt_nogap_len[ (ta>>48)&0xffffu ];

  if (seqlen-s-len==0) return(0.0);
  return((float)(count-len)/(float)(seqlen-s-len));
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
      dist_nogap_inc2(*p1,*p2,b4_m1,count,len);
     case 2:
      dist_nogap_inc2(*p1,*p2,b4_m2,count,len);
     case 3:
      dist_nogap_inc2(*p1,*p2,b4_m3,count,len);
     case 4:
      dist_nogap_inc2(*p1,*p2,b4_m4,count,len);
     case 5:
      dist_nogap_inc2(*p1,*p2,b4_m5,count,len);
     case 6:
      dist_nogap_inc2(*p1,*p2,b4_m6,count,len);
     case 7:
      dist_nogap_inc2(*p1,*p2,b4_m7,count,len);
     case 8:
      dist_nogap_inc2(*p1,*p2,b4_m8,count,len);
     case 9:
      dist_nogap_inc2(*p1,*p2,b4_m9,count,len);
     case 10:
      dist_nogap_inc2(*p1,*p2,b4_m10,count,len);
     case 11:
      dist_nogap_inc2(*p1,*p2,b4_m11,count,len);
     case 12:
      dist_nogap_inc2(*p1,*p2,b4_m12,count,len);
     case 13:
      dist_nogap_inc2(*p1,*p2,b4_m13,count,len);
     case 14:
      dist_nogap_inc2(*p1,*p2,b4_m14,count,len);
     case 15:
      dist_nogap_inc2(*p1,*p2,b4_m15,count,len);
      ++p1;
      ++p2;
    }
  }

  for (; p1<ep1; ++p1,++p2){
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


void calc_all_dists(int i,int s,int e,edoublearray& dists,int n,int tn)
{
  int l,j,k;

  int start=(n*arr.size())/tn;
  int end=((n+1)*arr.size())/tn;

  for (l=start; l<end; ++l)
    dists[l]=dist_nogap_short_compressed2(arr[i],arr[l],s,e);
//    dists[l]=dist_short_compressed(arr[i],arr[l],s,e);
}

float accCons(const efloatarray& avgcons,int s,int e)
{
  float res=0.0;
  for (; s<e; ++s) res+=(1.0-avgcons[s]);
  return(res);
}

class esite
{
 public:
  float a[6];
  esite() { clear(); }
  esite(const esite& s) { a[0]=s.a[0]; a[1]=s.a[1]; a[2]=s.a[2]; a[3]=s.a[3]; a[4]=s.a[4]; a[5]=s.a[5]; }
  esite& operator=(const esite& s) { a[0]=s.a[0]; a[1]=s.a[1]; a[2]=s.a[2]; a[3]=s.a[3]; a[4]=s.a[4]; a[5]=s.a[5]; return(*this); }
  estr getFrqs() const;
  estr getMostFrequent() const;
  inline float getTotal() const { return(a[0]+a[1]+a[2]+a[3]+a[4]+a[5]); }
  void clear() { a[0]=0.0; a[1]=0.0; a[2]=0.0; a[3]=0.0; a[4]=0.0; a[5]=0.0; }
};


estr esite::getFrqs() const
{
  estr tmp;
  char tmpstr[255];
  sprintf(tmpstr,"%2.2f",a[0]/getTotal());
  tmp=estr("A:")+tmpstr+" ";
  sprintf(tmpstr,"%2.2f",a[1]/getTotal());
  tmp+=estr("T:")+tmpstr+" ";
  sprintf(tmpstr,"%2.2f",a[2]/getTotal());
  tmp+=estr("G:")+tmpstr+" ";
  sprintf(tmpstr,"%2.2f",a[3]/getTotal());
  tmp+=estr("C:")+tmpstr+" ";
  sprintf(tmpstr,"%2.2f",a[4]/getTotal());
  tmp+=estr("-:")+tmpstr+" ";
  sprintf(tmpstr,"%2.2f",a[5]/getTotal());
  tmp+=estr("N:")+tmpstr+"";
  return("("+tmp+")");
}

estr esite::getMostFrequent() const
{
  char ch;
  unsigned int c;
  c=a[0]; ch='A';
  if (c<a[1]) { c=a[1]; ch='T'; }
  if (c<a[2]) { c=a[2]; ch='G'; }
  if (c<a[3]) { c=a[3]; ch='C'; }
  if (c<a[4]) { c=a[4]; ch='-'; }
  if (c<a[5]) { c=a[5]; ch='N'; }
  
  return(estr((char)ch) + " ("+ c/getTotal() +")");
}

class esiteProfile : public ebasicarray<esite>
{
 public:
  esiteProfile();
  esiteProfile(const esiteProfile& sp);
  void clear();
  void add(const estr& s,int slen);
  void add(const esiteProfile& sp);
};

esiteProfile::esiteProfile() {}

esiteProfile::esiteProfile(const esiteProfile& sp)
{
  init(sp.size());
  for (int i=0; i<sp.size(); ++i)
    at(i)=sp[i];
}

void esiteProfile::clear()
{
  for (int i=0; i<size(); ++i) at(i).clear();
}

void esiteProfile::add(const estr& s,int slen)
{
  if (size()==0) init(slen);
  int i;
  for (i=0; i<slen; ++i)
    ++at(i).a[getnuci(s,i)];
}

void esiteProfile::add(const esiteProfile& sp)
{
  int i,l;
  if (size()==0) init(sp.size());
  float total;
  for (i=0; i<sp.size(); ++i){
    total=sp[i].getTotal();
    for (l=0; l<6; ++l)
      at(i).a[l]+=sp[i].a[l]/total;
  }
}

eseqclusterData clusterData;

int emain()
{ 
  initLookupTable();

  cout << "# " << date() << endl;
  cout << "# " << args2str(argvc,argv) << endl;

  estr profile;
  epregister(profile);

  int winlen=70;
  epregister(winlen);
  int ncpus=4;
  epregister(ncpus);
  eparseArgs(argvc,argv);

  ldieif(argvc<2,"syntax: "+efile(argv[0]).basename()+" <long-seqs> [merge.log]");
  estrhashof<int> arrind;
  load_seqs_compressed(argv[1],arr,arrind,seqlen);

  int i,j,k,l,m;

  esiteProfile siteProfile;
  for (i=0; i<arr.size(); ++i)
    siteProfile.add(arr[i],seqlen);

  if (argvc>2) clusterData.load(efile(argv[2],"r"),arr.size());

  etaskman taskman;
  taskman.createThread(ncpus);

  cout << "# seqs: " << arr.size() << endl;
  earray<earray<eintarray> > otus;
  earray<eintarray> otusanc;
  efloatarray otuthres;
  for (float tmpf=0.95; tmpf>=0.50; tmpf-=0.05) otuthres.add(tmpf);
  clusterData.getTree(otuthres,otus,otusanc,arr.size());
  for (i=0; i<otuthres.size(); ++i)
    cout << "# otu(" << otuthres[i] <<"): " << otus[i].size() << endl;

  for (j=rnd.uniform()*arr.size(); j<arr.size(); ++j){
    edoublearray dists;
    dists.init(arr.size());
    cout << "# computing dists to: " << j << endl;
    for (i=0; i<ncpus; ++i)
      taskman.addTask(calc_all_dists,evararray((const int&)j,0,seqlen,dists,(const int&)i,(const int&)ncpus));
    taskman.wait();

    earray<efloatarray> otudists;
    earray<earray<esiteProfile> > otuprofiles;
    cout << "# computing otu dists/profiles for otus at: "<< otuthres[0] << endl;
    otudists.init(otuthres.size());
    otuprofiles.init(otuthres.size());
    otudists[0].init(otus[0].size(),0.0);
    otuprofiles[0].init(otus[0].size());
    for (i=0; i<otus[0].size(); ++i){
      for (l=0; l<otus[0][i].size(); ++l){
        otudists[0][i]+=dists[otus[0][i][l]];
        otuprofiles[0][i].add(arr[otus[0][i][l]],seqlen);
      }
      otudists[0][i]=otudists[0][i]/otus[0][i].size();
    }

    for (l=1; l<otuthres.size(); ++l){
      cout << "# computing otu dists/profiles for otus at: "<< otuthres[l] << endl;
      otudists[l].init(otus[l].size());
      otuprofiles[l].init(otus[l].size());
      for (i=0; i<otus[l].size(); ++i){
        for (k=0; k<otus[l][i].size(); ++k){
          otudists[l][i]+=otudists[l-1][otus[l][i][k]];
          otuprofiles[l][i].add(otuprofiles[l-1][otus[l][i][k]]);
        }
        otudists[l][i]=otudists[l][i]/otus[l][i].size();
      }
    }

    ebin2 bin;
    bin.xvalues.clear();
    double tmp;
    
    eintarray otuflag;
    otuflag.init(otus[0].size(),0);

    earray<esiteProfile> tmpotuprofiles;
    for (i=0; i<otudists[0].size(); ++i){
      if (otuflag[i]) continue;
      for (l=0; l<otuthres.size() && otudists[l][otusanc[l][i]]<otuthres[l]; ++l);
      if (l==otuthres.size()) l=otuthres.size()-1;
//      cout << l << " " << i << " " << otus[l].size() << " " << otusanc[l][i] << endl;
      tmpotuprofiles.add(otuprofiles[l][otusanc[l][i]]);
      bin.xvalues.add(otudists[l][otusanc[l][i]]);
      for (k=0; k<otusanc[l].size(); ++k)
        if (otusanc[l][k]==otusanc[l][i]) otuflag[i]=1;
    }
    bin.yvalues.init(bin.xvalues.size());
    for (k=0; k<seqlen; ++k){
      for (i=0; i<tmpotuprofiles.size(); ++i)
        bin.yvalues[i]=1.0-tmpotuprofiles[i][k].a[getnuci(arr[j],k)]/tmpotuprofiles[i][k].getTotal();
      bin.calc(15);
      esiteProfile binProfile;
      binProfile.init(seqlen);
      for (i=0,m=0; i<bin.xvalues.size(); ++i){
        if (m!=int((bin.xvalues[bin.iv[i]]-bin.xmin)*15/(bin.xmax-bin.xmin))) {
          cout << j << " " << k << " " << bin.xbin[m] << " " << bin.ybin[m] << " " << bin.ycount[m] << " " << getnuc(arr[j],k) << " " << binProfile[k].getFrqs() << " " << siteProfile[k].getFrqs() << endl;
          m=int((bin.xvalues[bin.iv[i]]-bin.xmin)*15/(bin.xmax-bin.xmin));
          binProfile.clear();
        }
        binProfile.add(tmpotuprofiles[bin.iv[i]]);
      }
      cout << endl;
    }
    break;
  }
  return(0);
}

