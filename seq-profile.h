#ifndef SEQ_PROFILE_H
#define SEQ_PROFILE_H

#include <eutils/estr.h>
#include <eutils/evar.h>
#include <eutils/earray.h>
#include <eutils/ebasicarray.h>

void mutateSeqLength(estr& seqstr,double mutrate,int length);
void mutateSeqs(earray<estr>& seqstr,double mutrate);
void mutateseq(estr& seqstr,int p);
void profileMutate(estr& seq,earray<ebasicarray<float> >& profile,float mutrate,long int& mutpos);
void profileMutateTimes(estr& seq,earray<ebasicarray<float> >& profile,double mutrate,double ntimes);
void profileMutateTimes(estr& seq,earray<ebasicarray<float> >& profile,float mutrate,long int& mutpos,int ntimes);
void profileInitSeq(estr& seq,earray<ebasicarray<float> >& profile);
char getMutation(ebasicarray<float>& bpp);
void profileLoad(earray<ebasicarray<float> >& profile,const estr& filename);

#endif /* SEQ_PROFILE_H */
