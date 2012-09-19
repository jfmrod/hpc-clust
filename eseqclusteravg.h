#ifndef ESEQCLUSTERAVG_H
#define ESEQCLUSTERAVG_H

#include <eutils/evar.h>
#include <eutils/estr.h>
#include <eutils/efile.h>
#include <eutils/eblockarray.h>
#include <eutils/ebasicarray.h>
#include "eseqcluster.h"
#include "eseqclustercount.h"
#include <set>

#undef check

typedef ebasichashmap<eseqdistCount,eseqdistCount,hash_lookup3_eseqdistCount> eseqdistavghash;

class eseqclusteravg
{
 public:
  int mergecount;
  efile ofile;
  float incmaxdist;
  float lastdist;

  eintarray scount;
  eintarray scluster;
  eintarray smerge;

  eseqdistavghash smatrix;
  ebasicarray<list<int> > inter;
  ebasicarray<list<int> > incluster;

  eseqclusterData clusterData;

  multiset<eseqdistCount,greater<eseqdistCount> > completemerges;

  void check(ebasicarray<eseqdistCount>& dists);

  eseqclusteravg();

  void init(int count,const estr& ofile,const estr& seqsfile,const earray<eintarray>& dupslist);

  void merge(const eseqdistCount& dist);

  void add(const eseqdist& sdist);
  void add(const eseqdistCount& sdist);
//  void add(int ind);

  long int update(eblockarray<eseqdistCount>& dists,long int s);

  float getIncompleteMaxDist(float newdist);
  void mergeComplete(float dist);
  void clearComplete();

  void save(const estr& filename,const estrarray& arr);
};


#endif

