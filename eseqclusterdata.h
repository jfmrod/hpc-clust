#ifndef ESEQCLUSTERDATA_H
#define ESEQCLUSTERDATA_H

class eseqclusterData
{
 public:
  int count;
  earray<eseqdist> mergearr;

  eseqclusterData();

  int getOTU(float dist,eintarray& otuarr);

  float getMergeDistance(int x,int y);
  void getCluster(const eintarray& seqs, eintarray& seqcluster, float& cdist);

  void save(const efile& f);
  void load(const efile& f,int count);
};

#endif

