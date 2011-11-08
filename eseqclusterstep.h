#ifndef ESEQCLUSTERSTEP_H
#define ESEQCLUSTERSTEP_H

#include <eutils/evar.h>
#include <eutils/estr.h>
#include <eutils/ethread.h>
#include <eutils/eblockarray.h>
#include <eutils/ebasicarray.h>

#undef check

#include "eseqcluster.h"

class eseqclusterstep
{
 public:
  int mergecount;
  efile ofile;

  emutex mutexDists;
  eblockarray<eseqdist> dists;

  eintarray scount;
  eintarray scluster;
  eintarray smerge;

  ebasicarray<list<int> > incluster;

  eseqclusterstep();

  void merge(int x,int y,float dist);
  void init(int count,const estr& ofile,const estr& seqsfile);
  void add(int ind);
  void add(eseqdist& sdist);

  void save(const estr& filename,const estrarray& arr);
};


#endif

