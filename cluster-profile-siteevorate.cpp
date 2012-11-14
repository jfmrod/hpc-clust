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

inline bool dist_nogap_short_compressed_single(const estr& s1,const estr& s2,int s)
{
  unsigned long int *p1=(long unsigned int*)(s1._str)+(s/16);
  unsigned long int *p2=(long unsigned int*)(s2._str)+(s/16);
  unsigned long m=(0x0ful<< (s%16)*4);
  if ((*p1)&(*p2)&m == m) return(false);
  if (((*p1)^(*p2))&m) return(false);
  return(true);
}

inline float dist_nogap_short_compressed2(const estr& s1,const estr& s2,int s,int seqlen)
{
  int len=0;
  int count=0;
  unsigned long int *ep1=(long unsigned int*)(s1._str)+MIN(s1._strlen/8,seqlen/16);
  unsigned long int *p1=(long unsigned int*)(s1._str)+(s/16);
  unsigned long int *p2=(long unsigned int*)(s2._str)+(s/16);
  unsigned long tx,ta;

  if (p1<ep1 && s%16){
    unsigned long m=~(0xfffffffffffffffful << (s%16)*4);
    tx=((*p1) ^ (*p2))&m;
    ta=((*p1) & (*p2))&m;
    count+=lt_nogap_count[ tx&0xffffu ] + lt_nogap_count[ (tx>>16)&0xffffu ] + lt_nogap_count[ (tx>>32)&0xffffu ] + lt_nogap_count[ (tx>>48)&0xffffu ] - (16-s%16);
    len+=lt_nogap_len[ ta&0xffffu ] + lt_nogap_len[ (ta>>16)&0xffffu ] + lt_nogap_len[ (ta>>32)&0xffffu ] + lt_nogap_len[ (ta>>48)&0xffffu ];
/*
    switch (s%16){
     case 1:
      dist_nogap_inc3(*p1,*p2,b4_m1,count,len);
     case 2:
      dist_nogap_inc3(*p1,*p2,b4_m2,count,len);
     case 3:
      dist_nogap_inc3(*p1,*p2,b4_m3,count,len);
     case 4:
      dist_nogap_inc3(*p1,*p2,b4_m4,count,len);
     case 5:
      dist_nogap_inc3(*p1,*p2,b4_m5,count,len);
     case 6:
      dist_nogap_inc3(*p1,*p2,b4_m6,count,len);
     case 7:
      dist_nogap_inc3(*p1,*p2,b4_m7,count,len);
     case 8:
      dist_nogap_inc3(*p1,*p2,b4_m8,count,len);
     case 9:
      dist_nogap_inc3(*p1,*p2,b4_m9,count,len);
     case 10:
      dist_nogap_inc3(*p1,*p2,b4_m10,count,len);
     case 11:
      dist_nogap_inc3(*p1,*p2,b4_m11,count,len);
     case 12:
      dist_nogap_inc3(*p1,*p2,b4_m12,count,len);
     case 13:
      dist_nogap_inc3(*p1,*p2,b4_m13,count,len);
     case 14:
      dist_nogap_inc3(*p1,*p2,b4_m14,count,len);
     case 15:
      dist_nogap_inc3(*p1,*p2,b4_m15,count,len);
      ++p1;
      ++p2;
    }
*/
    ++p1; ++p2;
  }

  for (; p1<ep1; ++p1,++p2){
    tx=((*p1) ^ (*p2));
    ta=((*p1) & (*p2));
    count+=lt_nogap_count[ tx&0xffffu ] + lt_nogap_count[ (tx>>16)&0xffffu ] + lt_nogap_count[ (tx>>32)&0xffffu ] + lt_nogap_count[ (tx>>48)&0xffffu ];
    len+=lt_nogap_len[ ta&0xffffu ] + lt_nogap_len[ (ta>>16)&0xffffu ] + lt_nogap_len[ (ta>>32)&0xffffu ] + lt_nogap_len[ (ta>>48)&0xffffu ];
  }

  tx=((*p1) ^ (*p2));
  ta=((*p1) & (*p2));
  count+=lt_nogap_count[ tx&0xffffu ] + lt_nogap_count[ (tx>>16)&0xffffu ] + lt_nogap_count[ (tx>>32)&0xffffu ] + lt_nogap_count[ (tx>>48)&0xffffu ] - (16-seqlen%16);
  len+=lt_nogap_len[ ta&0xffffu ] + lt_nogap_len[ (ta>>16)&0xffffu ] + lt_nogap_len[ (ta>>32)&0xffffu ] + lt_nogap_len[ (ta>>48)&0xffffu ];

/*
  switch (seqlen%16){
   case 15:
    dist_nogap_inc3(*p1,*p2,b4_m14,count,len);
   case 14:
    dist_nogap_inc3(*p1,*p2,b4_m13,count,len);
   case 13:
    dist_nogap_inc3(*p1,*p2,b4_m12,count,len);
   case 12:
    dist_nogap_inc3(*p1,*p2,b4_m11,count,len);
   case 11:
    dist_nogap_inc3(*p1,*p2,b4_m10,count,len);
   case 10:
    dist_nogap_inc3(*p1,*p2,b4_m9,count,len);
   case 9:
    dist_nogap_inc3(*p1,*p2,b4_m8,count,len);
   case 8:
    dist_nogap_inc3(*p1,*p2,b4_m7,count,len);
   case 7:
    dist_nogap_inc3(*p1,*p2,b4_m6,count,len);
   case 6:
    dist_nogap_inc3(*p1,*p2,b4_m5,count,len);
   case 5:
    dist_nogap_inc3(*p1,*p2,b4_m4,count,len);
   case 4:
    dist_nogap_inc3(*p1,*p2,b4_m3,count,len);
   case 3:
    dist_nogap_inc3(*p1,*p2,b4_m2,count,len);
   case 2:
    dist_nogap_inc3(*p1,*p2,b4_m1,count,len);
   case 1:
    dist_nogap_inc3(*p1,*p2,b4_m0,count,len);
  }
*/
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

  if (argvc>2) clusterData.load(efile(argv[2],"r"),arr.size());

  etaskman taskman;
  taskman.createThread(ncpus);

  cout << "# seqs: " << arr.size() << endl;
  earray<eintarray> otus;
  clusterData.getOTU(0.95,otus,arr.size());
  cout << "# otus: " << otus.size() << endl;

  int i,j,k,l;
  for (j=rnd.uniform()*arr.size(); j<arr.size(); ++j){
    edoublearray dists;
    dists.init(arr.size());
    for (i=0; i<ncpus; ++i)
      taskman.addTask(calc_all_dists,evararray((const int&)j,0,seqlen,dists,(const int&)i,(const int&)ncpus));
    taskman.wait();

    edoublearray mfrq;
    mfrq.init(otus.size(),0.0);

    ebin2 bin;
    bin.xvalues.init(otus.size(),0.0);
    bin.yvalues.init(otus.size());
    for (i=0; i<otus.size(); ++i){
      for (l=0; l<otus[i].size(); ++l)
        bin.xvalues[i]+=dists[otus[i][l]];
      bin.xvalues[i]=bin.xvalues[i]/otus[i].size();
    }

    for (k=0; k<seqlen; ++k){
      for (i=0; i<otus.size(); ++i){
        double tmp=0.0;
        for (l=0; l<otus[i].size(); ++l){
          if (!dist_nogap_short_compressed_single(arr[otus[i][l]],arr[j],k))
            tmp+=1.0;
        }
        tmp=tmp/otus[i].size();
        bin.yvalues[i]=tmp;

/*
        if (tmp>=0.5)
          bin.yvalues[i]=1;
        else
          bin.yvalues[i]=0;
*/
      }
      bin.calc(15);

      for (i=0; i<bin.xbin.size(); ++i){
        if (bin.ycount[i]==0) continue;
        cout << k << " " << bin.xbin[i] << " " << bin.ybin[i] << " " << bin.ycount[i] << endl;
      }
      cout << endl;
    }
    break;
  }
  return(0);
}

