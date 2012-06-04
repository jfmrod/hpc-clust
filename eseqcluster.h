#ifndef ESEQCLUSTER_H
#define ESEQCLUSTER_H

#include <eutils/evar.h>
#include <eutils/estr.h>
#include <eutils/efile.h>
#include <eutils/ethread.h>
#include <eutils/eblockarray.h>
#include <eutils/ebasicarray.h>


class eseqdist
{
 public:
  float dist;
//  int count;
  int x;
  int y;

  eseqdist();
  eseqdist(int x,int y,float dist);

  inline bool operator<(const eseqdist& sdist) const{ return(dist<sdist.dist); }
  void serial(estr& data) const;
  int unserial(const estr& data,int i);
};

#include "eseqclusterdata.h"

class eseqcluster
{
 public:
  efile ofile;

  int mergecount;

//  emutex mutexDists;
//  eblockarray<eseqdist> dists;
  eseqclusterData clusterData;

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

  void merge(int x,int y,float dist);
  void init(int count,const estr& ofile,const estr& seqsfile);
//  void add(int ind);
  void add(const eseqdist& sdist);

//  int update(ebasicarray<eseqdist>& dists,int s);

  void save(const estr& filename,const estrarray& arr);
//  eseqdist* operator()(int x,int y);
};


#endif

