#ifndef ESEQCLUSTERDATA_H
#define ESEQCLUSTERDATA_H

#include "eseqcluster.h"

class eseqclusterData
{
 public:
  earray<eseqdist> mergearr;

  float getMergeDistance(int x,int y);
  void save(const efile& f);
  void load(const efile& f);
};

#endif

