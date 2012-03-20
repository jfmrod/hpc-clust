#include <eutils/emain.h>
#include <eutils/efile.h>
#include <eutils/eheap.h>
#include <eutils/etime.h>
#include <eutils/etimer.h>
#include <eutils/eoption.h>

#include "cluster-common.h"

estrarray arr;
estrarray arrshort;

int seqlen=0;

estr args2str(int argvc,char **argv)
{
  estr tmpstr;
  int i;
  for (i=0; i<argvc; ++i)
    tmpstr+=estr(argv[i])+" ";
  tmpstr.del(-1);
  return(tmpstr);
}

inline float dist_short_compressed(const estr& s1,const estr& s2,int s,int seqlen)
{
  int len=seqlen;
  int count=0;
  long int *ep1=(long int*)(s1._str)+MIN(s1._strlen/8,seqlen/16);
  long int *p1=(long int*)(s1._str)+(s/16);
  long int *p2=(long int*)(s2._str)+(s/16);

  if (p1<ep1){
    switch (s%16){
     case 1:
      dist_inc(*p1,*p2,b4_m1,count);
     case 2:
      dist_inc(*p1,*p2,b4_m2,count);
     case 3:
      dist_inc(*p1,*p2,b4_m3,count);
     case 4:
      dist_inc(*p1,*p2,b4_m4,count);
     case 5:
      dist_inc(*p1,*p2,b4_m5,count);
     case 6:
      dist_inc(*p1,*p2,b4_m6,count);
     case 7:
      dist_inc(*p1,*p2,b4_m7,count);
     case 8:
      dist_inc(*p1,*p2,b4_m8,count);
     case 9:
      dist_inc(*p1,*p2,b4_m9,count);
     case 10:
      dist_inc(*p1,*p2,b4_m10,count);
     case 11:
      dist_inc(*p1,*p2,b4_m11,count);
     case 12:
      dist_inc(*p1,*p2,b4_m12,count);
     case 13:
      dist_inc(*p1,*p2,b4_m13,count);
     case 14:
      dist_inc(*p1,*p2,b4_m14,count);
     case 15:
      dist_inc(*p1,*p2,b4_m15,count);
      ++p1;
      ++p2;
    }
  }

  for (; p1<ep1; ++p1,++p2){
    dist_inc(*p1,*p2,b4_m0,count);
    dist_inc(*p1,*p2,b4_m1,count);
    dist_inc(*p1,*p2,b4_m2,count);
    dist_inc(*p1,*p2,b4_m3,count);
    dist_inc(*p1,*p2,b4_m4,count);
    dist_inc(*p1,*p2,b4_m5,count);
    dist_inc(*p1,*p2,b4_m6,count);
    dist_inc(*p1,*p2,b4_m7,count);
    dist_inc(*p1,*p2,b4_m8,count);
    dist_inc(*p1,*p2,b4_m9,count);
    dist_inc(*p1,*p2,b4_m10,count);
    dist_inc(*p1,*p2,b4_m11,count);
    dist_inc(*p1,*p2,b4_m12,count);
    dist_inc(*p1,*p2,b4_m13,count);
    dist_inc(*p1,*p2,b4_m14,count);
    dist_inc(*p1,*p2,b4_m15,count);
  }

  switch (seqlen%16){
   case 15:
    dist_inc(*p1,*p2,b4_m14,count);
   case 14:
    dist_inc(*p1,*p2,b4_m13,count);
   case 13:
    dist_inc(*p1,*p2,b4_m12,count);
   case 12:
    dist_inc(*p1,*p2,b4_m11,count);
   case 11:
    dist_inc(*p1,*p2,b4_m10,count);
   case 10:
    dist_inc(*p1,*p2,b4_m9,count);
   case 9:
    dist_inc(*p1,*p2,b4_m8,count);
   case 8:
    dist_inc(*p1,*p2,b4_m7,count);
   case 7:
    dist_inc(*p1,*p2,b4_m6,count);
   case 6:
    dist_inc(*p1,*p2,b4_m5,count);
   case 5:
    dist_inc(*p1,*p2,b4_m4,count);
   case 4:
    dist_inc(*p1,*p2,b4_m3,count);
   case 3:
    dist_inc(*p1,*p2,b4_m2,count);
   case 2:
    dist_inc(*p1,*p2,b4_m1,count);
   case 1:
    dist_inc(*p1,*p2,b4_m0,count);
  }
  return((float)count/(float)(seqlen-s));
}



void calc_best_dist(const estr& shortseq,const estrarray& seqs,int seqlen,int& bestmatch,double& bestdist)
{
  bestmatch=-1; bestdist=0.0;

  int s,e;
  for (s=0; s<shortseq.len() && (unsigned char)shortseq[s]==0xFFu; ++s);
  e=shortseq.len()-1;
  if (shortseq.len()>0 && shortseq.len()%2==1 && (unsigned char)shortseq[e]&0x0F==0x0F) --e;
  for (; e>0 && (unsigned char)shortseq[e]==0xFFu; --e);
  if (s>=e) return; 
  
//  cout << "s: " << s << " e: " << e << " len: "<<shortseq.len() << endl;

  double tmpdist;
  int i;
  for (i=0; i<seqs.size(); ++i){
    tmpdist=dist_short_compressed(shortseq,seqs.values(i),s,e);
    if (tmpdist>bestdist){ bestdist=tmpdist; bestmatch=i; }
  }
}

int emain()
{ 
  cout << "# " << date() << endl;
  cout << "# " << args2str(argvc,argv) << endl;

/*
  eoption<efunc> dfunc;
  dfunc.choice=0;
  dfunc.add("gap",t_calc_dists<estrarray,eseqdist,eblockarray<eseqdist>,dist_compressed>);
  dfunc.add("nogap",t_calc_dists<estrarray,eseqdist,eblockarray<eseqdist>,dist_nogap_compressed>);
  dfunc.add("tamura",t_calc_dists<estrarray,eseqdist,eblockarray<eseqdist>,dist_tamura_compressed>);
  dfunc.add("gap+noise",t_calc_dists_noise<estrarray,eseqdist,eblockarray<eseqdist>,dist_compressed>);
  dfunc.add("nogap+noise",t_calc_dists_noise<estrarray,eseqdist,eblockarray<eseqdist>,dist_nogap_compressed>);
  dfunc.add("tamura+noise",t_calc_dists_noise<estrarray,eseqdist,eblockarray<eseqdist>,dist_tamura_compressed>);

  epregisterClass(eoption<efunc>);
  epregisterClassMethod2(eoption<efunc>,operator=,int,(const estr& val));

  epregister(dfunc);
*/

  estr ofile="cluster-results";
  float t=0.90;
  epregister(t);
  epregister(ofile);
  eparseArgs(argvc,argv);


//  cout << "# distance function: " << dfunc.key() << endl;

  ldieif(argvc<3,"syntax: "+efile(argv[0]).basename()+" <long-seqs> <short-seqs>");

  int i;
  load_seqs_compressed(argv[1],arr,seqlen);
  load_short_compressed(argv[2],arrshort,seqlen);
  
  cout << "# computing distances" << endl;
  int bestmatch;
  double bestdist;
  for (i=0; i<arrshort.size(); ++i){
    calc_best_dist(arrshort.values(i),arr,seqlen,bestmatch,bestdist);
    if (bestmatch==-1) continue;
    cout << arrshort.keys(i) << " " << bestdist << " " << arr.keys(bestmatch) << endl;
  }

  return(0);
}

