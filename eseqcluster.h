#ifndef ESEQCLUSTER_H
#define ESEQCLUSTER_H

#include <eutils/evar.h>
#include <eutils/estr.h>
#include <eutils/ethread.h>
#include <eutils/eblockarray.h>
#include <eutils/ebasicarray.h>

#undef check

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

class eseqcluster
{
 public:
  int mergecount;

  emutex mutexDists;
  eblockarray<eseqdist> dists;

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
  void add(int ind);
  void add(eseqdist& sdist);

  void calc(estrarray& arr,int seqlen,int node,int tnodes,float thres);
  void calcGap(estrarray& arr,int seqlen,int node,int tnodes,float thres);
  void calcTamura(estrarray& arr,int seqlen,int node,int tnodes,float thres);

//  int update(ebasicarray<eseqdist>& dists,int s);

  void save(const estr& filename,const estrarray& arr);
//  eseqdist* operator()(int x,int y);
};


#endif

