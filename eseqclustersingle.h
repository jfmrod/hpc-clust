#ifndef ESEQCLUSTERSINGLE_H
#define ESEQCLUSTERSINGLE_H

#include <eutils/evar.h>
#include <eutils/estr.h>
#include <eutils/ethread.h>
#include <eutils/eblockarray.h>
#include <eutils/ebasicarray.h>

#undef check

#include "eseqcluster.h"

class eseqclustersingle
{
 public:
  int mergecount;

  emutex mutexDists;
  eblockarray<eseqdist> dists;

  eintarray scount;
  eintarray scluster;
  eintarray smerge;

  ebasicarray<list<int> > incluster;

  eseqclustersingle();

  void merge(int x,int y,float dist);
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

