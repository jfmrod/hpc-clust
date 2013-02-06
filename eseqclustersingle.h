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
  efile ofile;

//  emutex mutexDists;
//  eblockarray<eseqdist> dists;
  eseqclusterData clusterData;

  eintarray scount;
  eintarray scluster;
  eintarray smerge;

  ebasicarray<list<int> > incluster;

  eseqclustersingle();

  void merge(int x,int y,float dist);
  void init(int count,const estr& ofile,const estr& seqsfile,const earray<eintarray>& dupslist);
  void add(int ind);
  void add(const eseqdist& sdist);

//  int update(ebasicarray<eseqdist>& dists,int s);

  void save(const estr& filename,const estrarray& arr);
//  eseqdist* operator()(int x,int y);
};


#endif

