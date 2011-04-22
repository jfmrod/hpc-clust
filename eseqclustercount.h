#ifndef ESEQCLUSTER_MERGE_H
#define ESEQCLUSTER_MERGE_H

#include <eutils/evar.h>
#include <eutils/estr.h>
#include <eutils/ethread.h>
#include <eutils/eblockarray.h>
#include <eutils/ebasicarray.h>

#undef check

class eseqdistCount
{
 public:
  float dist;
  int count;
  int x;
  int y;

  eseqdistCount();
  eseqdistCount(int x,int y,float dist);

  inline bool operator<(const eseqdistCount& sdist) const{ return(dist<sdist.dist?true:false); }
  void serial(estr& data) const;
  int unserial(const estr& data,int i);
};

class eseqclusterCount
{
 public:
  int mergecount;

  emutex mutexDists;
  eblockarray<eseqdistCount> dists;

  eintarray scount;
  eintarray scluster;
  eintarray smerge;

  ebasicstrhashof<int> smatrix;
  ebasicarray<list<int> > inter;
  ebasicarray<list<int> > incluster;

  void check(ebasicarray<eseqdistCount>& dists);

  eseqclusterCount();

  void merge(int x,int y);
  void init(int count);
  void add(int ind);

  void calc(estrarray& arr,int seqlen,int node,int tnodes,float thres);

  int update(int ind);
  int update(int ind,int x,int y);
  int update(eblockarray<eseqdistCount>& dists,int s);

  void save(const estr& filename,const estrarray& arr);
};


#endif

