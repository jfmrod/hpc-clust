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
  INDTYPE mergecount;
  INDTYPE cf;

  float thres;
  int seqlen;
  estrarray *seqarr;
  float (*fdist)(const estr&,const estr&,int);

  efile ofile;
  eseqdistavghash::iter incmaxit;
  float incmaxdist;
  float lastdist;

  ebasicarray<INDTYPE> scount;
  ebasicarray<INDTYPE> scluster;
  ebasicarray<INDTYPE> smerge;

  eseqdistavghash smatrix;
  ebasicarray<list<INDTYPE> > inter;
  ebasicarray<list<INDTYPE> > incluster;

  eseqclusterData clusterData;

  multiset<eseqdistCount,greater<eseqdistCount> > completemerges;

  void check(ebasicarray<eseqdistCount>& dists);

  eseqclusteravg();

  void init(INDTYPE count,const estr& ofile,const estr& seqsfile,const earray<ebasicarray<INDTYPE> >& dupslist,float thres,float (fdist)(const estr&,const estr&,int),estrarray& arr,int seqlen);

  void finalize();

  void merge(const eseqdistCount& dist);

  void add(const eseqdist& sdist);
  void add(const eseqdistCount& sdist);
//  void add(int ind);

  long update(eblockarray<eseqdistCount>& dists,long s);

//  float getIncompleteMaxDist(float newdist);
  void getIncompleteMaxDist(float newdist,float &maxdist,eseqdistavghash::iter& maxit);
  void mergeComplete(float dist);
  void clearComplete();

  void save(const estr& filename,const estrarray& arr);
};


#endif

