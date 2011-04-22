#ifndef CLUSTER_COMMON_H
#define CLUSTER_COMMON_H

#include <eutils/evar.h>
#include <eutils/estr.h>
#include <eutils/ebasicarray.h>
#include <eutils/eintarray.h>
#include <eutils/ebasichashmap.h>
#include <eutils/estrhash.h>
#include <eutils/etimer.h>
#include <eutils/eblockarray.h>

#include <list>

#include <eutils/ethread.h>

#include "eseqcluster.h"
#include "eseqclustercount.h"

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


inline void dist_nogap_inc(long int a1,long int a2,long int mask,int& count,int& len){
  if ((a1&mask)==mask && (a2&mask)==mask)
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

//inline void dist_nogap_inc(long int a1,long int a2,long int mask,int& count,int& len){
//  if (!((a1&mask)==mask || (a2&mask)==mask)){
//    if ((a1&mask)==(a2&mask)) ++count;
//    ++len;
//  }
//}

inline float dist_nogap_compressed(const estr& s1,const estr& s2,int seqlen)
{
  int len=seqlen;
  int count=0;
  long int *ep1=(long int*)(s1._str)+(s1._strlen/8);
  long int *p1=(long int*)s1._str;
  long int *p2=(long int*)s2._str;
  for (; p1!=ep1; ++p1,++p2){
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
  if (neigharr[i].size()==0 || neigharr[j].size()==0) return(1.0);
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
class eseqcount
{
 public:
  int x;
  int y;
  bool operator==(const eseqcount& scount) const;
};
*/


/*

class eblockarray
{
 private:
  long int blocksize;
  long int count;
 public:
  ebasicarray<eseqdist*> blocks;

  eblockarray();
  ~eblockarray();

  void clear();

  void add(const eseqdist& sdist);
  inline long int size() { return(count); }

  void swap(long int i,long int j);
  void sort();

  eseqdist& operator[](long int i);
  const eseqdist& operator[](long int i) const;
  eblockarray& merge(eblockarray& barr);

  inline eseqdist* lastblock() { return(blocks[blocks.size()-1]); }
  inline const eseqdist* lastblock() const { return(blocks[blocks.size()-1]); }
};

*/



int calc_dists(estrarray& arr,earray<eseqdist>& dists,int node,int tnodes,float thres);
int calc_dists(estrarray& arr,ebasicarray<eseqdist>& dists,int node,int tnodes,float thres);
//int calc_dists(estrhash& arr,earray<eseqdist>& dists,int start,int end,float thres);
//int calc_dists2(estrarray& arr,earray<eseqdist>& dists,int node,int tnodes,float thres);

//int calc_dists_nogap(estrarray& arr,multimap<float,eseqdist>& dists,int node,int tnodes,float thres);
int calc_dists_nogap_compressed(earray<estr>& arr,ebasicarray<eseqdist>& dists,int seqlen,int node,int tnodes,float thres);
int calc_dists_nogap_compressed(estrarray& arr,ebasicarray<eseqdist>& dists,int seqlen,int node,int tnodes,float thres);
int calc_dists_nogap_compressed(earray<estr>& arr,eblockarray<eseqdist>& dists,int seqlen,int node,int tnodes,float thres);
int calc_dists_nogap_compressed(estrarray& arr,eblockarray<eseqdist>& dists,int seqlen,int node,int tnodes,float thres);
int calc_dists_nogap_compressed(estrarray& arr,eblockarray<eseqdistCount>& dists,int seqlen,int node,int tnodes,float thres);

int calc_dists_nogap(estrarray& arr,ebasicarray<eseqdist>& dists,int node,int tnodes,float thres);
int calc_dists_nogap(estrarray& arr,earray<eseqdist>& dists,int node,int tnodes,float thres);
int calc_dists_nogap(estrarray& arr,eintarray& arrgaps,earray<eseqdist>& dists,int node,int tnodes,float thres);

int calc_dists_local(estrarray& arr,ebasicarray<float>& dists,float thres);

void cluster_init(earray<eintarray>& cvec,const estrhashof<int>& arrind,const eintarray& otuarr,int otucount);

void cooc_init(earray<eintarray>& neigharr,const estrhash& arr,const eintarray& otuarr,estrhashof<earray<estr> >& samples,estrhash& accsample,estrhashof<int>& arrind,int otucount);
void cooc_calc(int start,int end,ebasicarray<float>& dist_mat,earray<eintarray>& neigharr);

//void load_accs(const estr& filename,estrhash& arr);
//void load_seqs(const estr& filename,estrhash& arr);
void load_accs(const estr& filename,estrarray& arr);
void load_seqs(const estr& filename,estrarray& arr);
void load_seqs_compressed(const estr& filename,earray<estr>& arr,int& seqlen);
void load_seqs_compressed(const estr& filename,estrarray& arr,int& seqlen);
void load_seqs(const estr& filename,estrarray& arr,eintarray& arrgaps);
void load_seqs(const estr& filename,estrhash& arr);
void load_seqs(const estr& filename,estrhashof<int>& arrind);
void load_seqs(const estr& filename,estrarray& arr,estrhashof<int>& arrind);

void load_clusters(const estr& filename,estrhashof<int>& arrind,eintarray& otuarr,int& otucount);
void load_clusters(const estr& filename,estrhashof<int>& arrind,earray<eintarray>& otus);

void cluster_complete(const estrhash& arr,const ebasicarray<float>& dist_mat,const eintarray& dsort,earray<eintarray>& otus,eintarray& otuarr,float t);
void cluster_single(const estrhash& arr,const ebasicarray<float>& dist_mat,const eintarray& dsort,earray<eintarray>& otus,float t);


#endif /* CLUSTER_COMMON_H */

