#ifndef ESEQCLUSTER_H
#define ESEQCLUSTER_H

#include "hpc-clust-config.h"

#include <eutils/evar.h>
#include <eutils/estr.h>
#include <eutils/efile.h>
#include <eutils/ethread.h>
#include <eutils/eblockarray.h>
#include <eutils/ebasicarray.h>

#undef check

#ifdef HPC_CLUST_USE_LONGIND
 #define INDTYPE long
#else
 #define INDTYPE int
#endif

class eseqdist
{
 public:
  float dist;
//  int count;

  INDTYPE x;
  INDTYPE y;

  eseqdist();
  eseqdist(INDTYPE x,INDTYPE y,float dist);

  inline bool operator<(const eseqdist& sdist) const{ return(dist<sdist.dist); }
  void serial(estr& data) const;
  int unserial(const estr& data,int i);
};

#include "eseqclusterdata.h"

class eseqcluster
{
 public:
  efile ofile;

  INDTYPE mergecount;

//  emutex mutexDists;
//  eblockarray<eseqdist> dists;
  eseqclusterData clusterData;

  eintarray scount;
  eintarray scluster;
  eintarray smerge;

  ebasicstrhashof<INDTYPE> smatrix;
  ebasicarray<list<INDTYPE> > inter;
  ebasicarray<list<INDTYPE> > incluster;

  void check(ebasicarray<eseqdist>& dists);

  eseqcluster();

  void merge(INDTYPE x,INDTYPE y,float dist);
  void init(INDTYPE count,const estr& ofile,const estr& seqsfile,const earray<ebasicarray<INDTYPE> >& dupslist);
  void add(const eseqdist& sdist);

  void save(const estr& filename,const estrarray& arr);
};


#endif

