#include "seq-profile.h"

#include <eutils/eregexp.h>
#include <eutils/efile.h>
#include <eutils/ernd.h>


eregexp re_wspace("[ \t]+");


void profileLoad(earray< ebasicarray<float> >& profile,const estr& filename)
{
  efile f(filename);

  ldieif(!f.exists(),"file not found: "+filename);

  int i;
  estr line;
  while (f.readln(line)){
    if (line.len()==0 || line[0]=='#') continue;
    estrarray arr(re_explode(line,re_wspace));
    ldieif(arr.size()<10,"not enough elements in line: "+line);
    profile.add(ebasicarray<float>());
    ldieif(arr[0].i()!=(int)(profile.size()-1),"wrong id on line: "+line);
    profile[profile.size()-1].init(6);
    for (i=0; i<6; ++i)
      profile[profile.size()-1][i]=arr[i+3].f();
  }
}

char getMutation(ebasicarray<float>& bpp)
{
  float r=rnd.uniform()*(bpp[0]+bpp[1]+bpp[2]+bpp[3]);
  r-=bpp[0];
  if (r<0.0) return('A');
  r-=bpp[1];
  if (r<0.0) return('T');
  r-=bpp[2];
  if (r<0.0) return('G');
  return('C');
}

void profileInitSeq(estr& seq,earray<ebasicarray<float> >& profile)
{
  int i;
  seq.clear();
  for (i=0; i<profile.size(); ++i)
    seq+=getMutation(profile[i]);
}

void profileMutateTimes(estr& seq,earray<ebasicarray<float> >& profile,double mutrate,double ntimes)
{
  if (ntimes*mutrate==0.0l) return;

  long int nextmut;

  nextmut=rnd.uniform()*rnd.exponential(1.0l/(ntimes*mutrate)); // this ensures that the first mutation is randomly distributed
  ldieif(nextmut<0l,"nextmut is negative: "+estr(nextmut)+" mutrate: "+estr(mutrate)+" ntimes: "+estr(ntimes)+" prod: "+estr(mutrate*ntimes));
  while (nextmut<seq.len()){
    ldieif(nextmut<0l,"nextmut is negative2: "+estr(nextmut));
    seq[nextmut]=getMutation(profile[nextmut]);
    nextmut+=(1.0+rnd.exponential(1.0l/(ntimes*mutrate)));
  }
}

void profileMutateTimes(estr& seq,earray<ebasicarray<float> >& profile,float mutrate,long int& mutpos,int ntimes)
{
  for (;ntimes>0; --ntimes){
    while (mutpos<seq.len()){
      seq[mutpos]=getMutation(profile[mutpos]);
      mutpos+=rnd.exponential((double)1.0l/(ntimes*mutrate));
    }
    mutpos-=seq.len();
  }
}

void profileMutate(estr& seq,earray<ebasicarray<float> >& profile,float mutrate,long int& mutpos)
{
  while (mutpos<seq.len()){
    seq[mutpos]=getMutation(profile[mutpos]);
    mutpos+=rnd.exponential(1.0/mutrate);
  }
  mutpos-=seq.len();
}

void mutateseq(estr& seqstr,int p)
{
  double f=rnd.uniform();
  if (f<0.25)
    seqstr[p]='A';
  else if (f<0.50)
    seqstr[p]='T';
  else if (f<0.75)
    seqstr[p]='G';
  else
    seqstr[p]='C';
}


void mutateSeqs(earray<estr>& seqstr,double mutrate)
{
  long int nextmut,i;

  nextmut=rnd.uniform()*rnd.exponential(1.0/mutrate);
  for (i=0; i<seqstr.size(); ++i){
    while (nextmut<seqstr[i].len()) {
      mutateseq(seqstr[i],nextmut);
      nextmut+=rnd.exponential(1.0/mutrate);
    }
    nextmut-=seqstr[i].len();
  }
}

void mutateSeqLength(estr& seqstr,double mutrate,int length)
{
  long int nextmut,i;

  nextmut=rnd.uniform()*rnd.exponential((double)1.0/(length*mutrate)); // this ensures that the first mutation is randomly distributed
  while (nextmut<seqstr.len()) {
    mutateseq(seqstr,nextmut);
    nextmut+=rnd.exponential((double)1.0/(length*mutrate));
  }
  nextmut-=seqstr.len();
}


