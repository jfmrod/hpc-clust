#include <eutils/emain.h>
#include <eutils/efile.h>
#include <eutils/eheap.h>
#include <eutils/ethread.h>
#include <math.h>

#include "cluster-common.h"

class eseqpart
{
 public:
  int ref;
  eintarray seqs;
  ebasicarray<float> dists;
  
  float maxdist;
  float avgdist;

  earray<ebasicarray<float> > partsdists;
  earray<eseqpart> parts;
  ebasicarray<eseqpart*> intersects;

  eseqpart();
  eseqpart(int i);

  float getPartDist(int i,int j);

  void init(estrarray& arr);
  void partition(estrarray& arr,int level);
};

eseqpart::eseqpart(): ref(0),avgdist(0.0),maxdist(0.0){}
eseqpart::eseqpart(int i): ref(i),avgdist(0.0),maxdist(0.0){}


void eseqpart::init(estrarray& arr)
{
  int i;
  float tmpdist;

  ref=0;
  avgdist=0.0;

  for (i=1; i<arr.size(); ++i)
    seqs.add(i);
  
  for (i=1; i<arr.size(); ++i){
    tmpdist=1.0-dist(arr[0],arr[i]);
    if (tmpdist>maxdist) maxdist=tmpdist;
    avgdist+=tmpdist;
    dists.add(tmpdist);
  }
  avgdist=avgdist/seqs.size();
  partition(arr,0);
}

void eseqpart::partition(estrarray& arr,int level)
{
  int i,j;
  float tmpdist;
  int foundref;
  float foundrefdist;

  parts.add(eseqpart(ref));
  ebasicarray<float> *tmppartdist=new ebasicarray<float>();

  for (i=0; i<dists.size(); ++i){
    if (dists[i]<avgdist/2.0){
      parts[0].seqs.add(seqs[i]);
      parts[0].dists.add(dists[i]);
      parts[0].avgdist+=dists[i];
      if (dists[i]>parts[0].maxdist) parts[0].maxdist=dists[i];
    }else{
      tmppartdist->add(dists[i]);
      foundref=-1;
      foundrefdist=1.0;
      for (j=1; j<parts.size(); ++j){
        tmpdist=1.0-dist(arr[seqs[i]],arr[parts[j].ref]);
        tmppartdist->add(tmpdist);
        if (tmpdist<avgdist/2.0 && foundrefdist>tmpdist) { foundrefdist=tmpdist; foundref=j; }
      }
      if (foundref>0){
        parts[foundref].seqs.add(seqs[i]);
        parts[foundref].dists.add(foundrefdist);
        parts[foundref].avgdist+=foundrefdist;
        if (foundrefdist>parts[foundref].maxdist) parts[foundref].maxdist=foundrefdist;
      }else{
        parts.add(eseqpart(seqs[i]));
        partsdists.addref(tmppartdist);
        tmppartdist=new ebasicarray<float>();
      }
    }
  }

  for (i=0; i<level; ++i) cout <<"| ";

  if (intersects.size())
    cout << " \\- ";
  else
    cout << " \\+ ";
  cout << ref << " ("<<avgdist<<" < "<<maxdist<<") ["<<seqs.size()<< "]" << " {"<<parts.size()<<"} ^: "<< intersects.size() << endl;

//  if (level>0) return;

  for (i=0; i<parts.size(); ++i){
    if (parts[i].seqs.size())
      parts[i].avgdist=parts[i].avgdist/parts[i].seqs.size();
  }

  for (i=0; i<parts.size(); ++i){
    for (j=0; j<parts.size(); ++j){
      if (i==j) continue;
//      if (getPartDist(i,j)-parts[i].maxdist-parts[j].maxdist<parts[i].maxdist) parts[i].intersects=true; 
      if (getPartDist(i,j)-parts[i].maxdist-parts[j].maxdist<parts[i].maxdist) { parts[i].intersects.add(&parts[j]); }
    }
  }

//  if (level>0) return;


  for (i=0; i<parts.size(); ++i){
    if (parts[i].seqs.size())
      parts[i].partition(arr,level+1);
  }

}

float eseqpart::getPartDist(int i,int j)
{
  int tmp;
  if (i>j){ tmp=i; i=j; j=tmp; }
  return(partsdists[j-1][i]);
}

int emain()
{
  float ct=0.80;
  float rt=0.95;
  epregister(ct);
  epregister(rt);

  eparseArgs(argvc,argv);

  ldieif(argvc<2,"syntax: "+efile(argv[0]).basename()+" <seqali>");


  estrarray arr;

  load_seqs(argv[1],arr);

  eseqpart root;

  root.init(arr);

  return(0);
}

