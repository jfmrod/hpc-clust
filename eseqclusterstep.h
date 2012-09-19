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
  int step;

  emutex mutexDists;
  eblockarray<eseqdist> dists;

  eintarray scount;
  eintarray scluster;
  eintarray smerge;
  int otucount;
  earray<eintarray> otumembers;
  edoublearray      otudist;
  eintarray         otu;

  ebasicarray<list<int> > incluster;

  void merge(int x,int y,int dx,int dy,float dist);
  void init(int count,const estr& ofile,const estr& seqsfile,const earray<eintarray>& dupslist);
  void add(int ind);
  void add(eseqdist& sdist);

  void save(const estr& filename,const estrarray& arr);
};


#endif

