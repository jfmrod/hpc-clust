#ifndef ESEQ_H
#define ESEQ_H

#include <eutils/efile.h>
#include <eutils/estrhashof.h>

class eseq
{
 public:
  estr id;
  estr tax;
  estr itax;
  estr strain;
  estr isosrc;
  estr sample;
  estr organism;
//  earray<estr> dups;
  eseq() {}
  eseq(const estr& _id): id(_id) {}
};

void loadSeqs(estrhashof<eseq>& seqs,const efile& f);
void loadSeqsInfo(estrhashof<eseq>& seqs,const efile& f);


#endif
