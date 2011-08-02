#ifndef ESEQCLUSTERCOUNT_H
#define ESEQCLUSTERCOUNT_H

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
  int count;
  int x;
  int y;

  eseqdistCount();
  eseqdistCount(int x,int y,float dist);

  inline bool operator==(const eseqdistCount& sdist) const{ return(x==sdist.x && y==sdist.y || x==sdist.y && y==sdist.x); }
  inline bool operator<(const eseqdistCount& sdist) const{ return(dist<sdist.dist); }
  inline bool operator>(const eseqdistCount& sdist) const{ return(dist>sdist.dist); }
  void serial(estr& data) const;
  int unserial(const estr& data,int i);
};

inline unsigned int hash_lookup3_eseqdistCount(const eseqdistCount& dist)
{
  int tmp[2];
  if (dist.x<dist.y)
    { tmp[0]=dist.x; tmp[1]=dist.y; }
  else
    { tmp[1]=dist.x; tmp[0]=dist.y; }
  return(hash_lookup3(tmp,sizeof(int)*2,0));
}

typedef ebasichashmap<eseqdistCount,int,hash_lookup3_eseqdistCount> eseqdisthash;

class eseqclusterCount
{
 public:
  int mergecount;

  emutex mutexDists;
  eblockarray<eseqdistCount> dists;

  eintarray scount;
  eintarray scluster;
  eintarray smerge;

  eseqdisthash smatrix;
  ebasicarray<list<int> > inter;
  ebasicarray<list<int> > incluster;

  void check(ebasicarray<eseqdistCount>& dists);

  eseqclusterCount();

  void merge(int x,int y);
  void init(int count);

  void add(eseqdistCount& sdist);
  void add(int ind);

  long int update(long int ind);
  long int update(long int ind,int x,int y);
  long int update(eblockarray<eseqdistCount>& dists,long int s);

  void save(const estr& filename,const estrarray& arr);
};

ostream& operator<<(ostream& strm,const eseqdistCount& sdist);

#endif

