#ifndef ESEQCLUSTERCOUNT_H
#define ESEQCLUSTERCOUNT_H

#include "hpc-clust-config.h"

#include <eutils/evar.h>
#include <eutils/estr.h>
#include <eutils/ethread.h>
#include <eutils/eblockarray.h>
#include <eutils/ebasicarray.h>

#undef check

class eseqdistCount
{
 public:
  float dist;
  long count;
  INDTYPE x;
  INDTYPE y;

  eseqdistCount();
  eseqdistCount(INDTYPE x,INDTYPE y,float dist);

  inline bool operator==(const eseqdistCount& sdist) const{ return(x==sdist.x && y==sdist.y || x==sdist.y && y==sdist.x); }
  inline bool operator<(const eseqdistCount& sdist) const{ return(dist<sdist.dist); }
  inline bool operator>(const eseqdistCount& sdist) const{ return(dist>sdist.dist); }
  void serial(estr& data) const;
  int unserial(const estr& data,int i);
};

inline size_t hash_lookup3_eseqdistCount(const eseqdistCount& dist)
{
  INDTYPE tmp[2];
  if (dist.x<dist.y)
    { tmp[0]=dist.x; tmp[1]=dist.y; }
  else
    { tmp[1]=dist.x; tmp[0]=dist.y; }
  return(hash_lookup3(tmp,sizeof(INDTYPE)*2,0));
}

typedef ebasichashmap<eseqdistCount,INDTYPE,hash_lookup3_eseqdistCount> eseqdisthash;

class eseqclusterCount
{
 public:
  INDTYPE mergecount;

  emutex mutexDists;
  eblockarray<eseqdistCount> dists;

  eseqclusterData clusterData;

  ebasicarray<INDTYPE> scount;
  ebasicarray<INDTYPE> scluster;
  ebasicarray<INDTYPE> smerge;

  eseqdisthash smatrix;
  ebasicarray<list<INDTYPE> > inter;
  ebasicarray<list<INDTYPE> > incluster;

  void check(ebasicarray<eseqdistCount>& dists);

  eseqclusterCount();

  void merge(INDTYPE x,INDTYPE y,float dist);
  void init(INDTYPE count);

  void add(eseqdistCount& sdist);
//  void add(int ind);

  long update(long ind);
  long update(long ind,INDTYPE x,INDTYPE y);
  long update(eblockarray<eseqdistCount>& dists,long s);

  void save(const estr& filename,const estrarray& arr);
};

ostream& operator<<(ostream& strm,const eseqdistCount& sdist);

#endif

