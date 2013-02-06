#ifndef ESEQCLUSTERDATA_H
#define ESEQCLUSTERDATA_H

#include "eseq.h"

class eseqclusterData
{
 public:
  int count;
  earray<eseqdist> mergearr;

  eseqclusterData();

  int getOTU(float dist,eintarray& otuarr);
  int getOTU(float dist,earray<eintarray>& otuarr,int size);
  int getTree(const efloatarray& dists,earray<earray<eintarray> >& otuarr,earray<eintarray>& otuanc,int size);

  void inferTaxonomy(estrhashof<eseq>& seqs);

  float getMergeDistance(int x,int y);
  void getCluster(const eintarray& seqs, eintarray& seqcluster, float& cdist);

  void save(const efile& f);
  void load(const efile& f,int count);
};

#endif

