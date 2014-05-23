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
  INDTYPE mergecount;
  efile ofile;

//  emutex mutexDists;
//  eblockarray<eseqdist> dists;
  eseqclusterData clusterData;

  ebasicarray<INDTYPE> scount;
  ebasicarray<INDTYPE> scluster;
  ebasicarray<INDTYPE> smerge;

  ebasicarray<list<INDTYPE> > incluster;

  eseqclustersingle();

  void merge(INDTYPE x,INDTYPE y,float dist);
  void init(INDTYPE count,const estr& ofile,const estr& seqsfile,const earray<ebasicarray<INDTYPE> >& dupslist);
  void add(INDTYPE ind);
  void add(const eseqdist& sdist);

//  int update(ebasicarray<eseqdist>& dists,int s);

  void save(const estr& filename,const estrarray& arr);
//  eseqdist* operator()(int x,int y);
};


#endif

