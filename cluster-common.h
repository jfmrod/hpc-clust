#ifndef CLUSTER_COMMON_H
#define CLUSTER_COMMON_H

#include <eutils/evar.h>
#include <eutils/estr.h>
#include <eutils/ebasicarray.h>
#include <eutils/eintarray.h>
#include <eutils/ebasichashmap.h>
#include <eutils/estrhash.h>
#include <eutils/etimer.h>

#include <list>

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

class eseqdist
{
 public:
  float dist;
//  int count;
  int x;
  int y;

  eseqdist();
  eseqdist(int x,int y,float dist);

  inline bool operator<(const eseqdist& sdist) const{ return(dist<sdist.dist?true:false); }
  void serial(estr& data) const;
  int unserial(const estr& data,int i);
};

class eseqcount
{
 public:
  int x;
  int y;
  bool operator==(const eseqcount& scount) const;
};

class eseqcluster
{
 public:
  int mergecount;

  eintarray scount;
  eintarray scluster;
  eintarray smerge;

//  ehashmap<eseqcount,int> smatrix;
  ebasicstrhashof<int> smatrix;

//  ebasicarray<eintarray> inter;
  ebasicarray<list<int> > inter;

//  ebasicarray<eintarray> incluster;
  ebasicarray<list<int> > incluster;

  void check(ebasicarray<eseqdist>& dists);

  eseqcluster();

  void merge(int x,int y);
  void init(int count);
  void add(eseqdist& dist);

//  int update(ebasicarray<eseqdist>& dists,int s);

  void save(const estr& filename,const estrarray& arr);
//  eseqdist* operator()(int x,int y);
};

int calc_dists(estrarray& arr,earray<eseqdist>& dists,int node,int tnodes,float thres);
int calc_dists(estrarray& arr,ebasicarray<eseqdist>& dists,int node,int tnodes,float thres);
//int calc_dists(estrhash& arr,earray<eseqdist>& dists,int start,int end,float thres);
//int calc_dists2(estrarray& arr,earray<eseqdist>& dists,int node,int tnodes,float thres);

//int calc_dists_nogap(estrarray& arr,multimap<float,eseqdist>& dists,int node,int tnodes,float thres);
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
void load_seqs(const estr& filename,estrarray& arr,eintarray& arrgaps);
void load_seqs(const estr& filename,estrhash& arr);
void load_seqs(const estr& filename,estrhashof<int>& arrind);
void load_clusters(const estr& filename,estrhashof<int>& arrind,eintarray& otuarr,int& otucount);

void cluster_complete(const estrhash& arr,const ebasicarray<float>& dist_mat,const eintarray& dsort,earray<eintarray>& otus,eintarray& otuarr,float t);
void cluster_single(const estrhash& arr,const ebasicarray<float>& dist_mat,const eintarray& dsort,earray<eintarray>& otus,float t);


#endif /* CLUSTER_COMMON_H */

