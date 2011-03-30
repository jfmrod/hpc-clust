#include <eutils/emain.h>
#include <eutils/efile.h>
#include <eutils/eheap.h>
#include <eutils/esystem.h>

#include "cluster-common.h"

estrarray arr;
estrhashof<int> arrind;
earray<eintarray> otus;
int otucount;

void printVec(const eintarray& arr)
{
  int i;
  for (i=0; i<arr.size(); ++i)
    cout << arr[i];
  cout << endl;
}

int otuSize(const eintarray& arr)
{
  int i,count;
  count=0;
  for (i=0; i<arr.size(); ++i)
    if (arr[i]>0) ++count;
  return(count);
}

float otuvec_dist(const eintarray& vec1,const eintarray& vec2)
{
  int i;
  int miss=0;
  int hit=0;
  ldieif(vec1.size()!=vec2.size(),"size mismatch");
  for (i=0; i<vec1.size(); ++i){
    if (vec1[i]>0){
      if (vec1[i]==vec2[i]) ++hit;
      else ++miss;
    }else if (vec2[i]>0)
      ++miss;
  }
  if (miss+hit==0){
    printVec(vec1);
    printVec(vec2);
  }
  ldieif(miss+hit==0,"miss=0, empty otu???");
  return((float)hit/(float)(hit+miss));
}

float calc_otu_intradist(eintarray& arr1)
{
  int i,j;
  float tmp;
  float min=1.0;
  for (i=0; i<arr1.size(); ++i){
    for (j=i+1; j<arr1.size(); ++j){
      tmp=dist_nogap(arr.values(arr1[i]),arr.values(arr1[j]));
      if (tmp<min) min=tmp;
    }
  }
  return(min);
}

float calc_otu_dist(eintarray& arr1,eintarray& arr2)
{
  int i,j;
  float tmp;
  float min=1.0;
  for (i=0; i<arr1.size(); ++i){
    for (j=0; j<arr2.size(); ++j){
      tmp=dist_nogap(arr.values(arr1[i]),arr.values(arr2[j]));
      if (tmp<min) min=tmp;
    }
  }
  return(min);
}

int emain()
{
  eparseArgs(argvc,argv);

  ldieif(argvc<3,"syntax: "+efile(argv[0]).basename()+" <seqs> <clusters1> <clusters2>");

  load_seqs(argv[1],arr,arrind);
  load_clusters(argv[2],arrind,otus);

  

//  earray<eintarray> otuvec;
//  cluster_init(otuvec,arrind,otuarr,otucount);

  int i,j;
  float tmp;
  int wrongCount=0;
  for (i=0; i<otus.size(); ++i){
    for (j=i+1; j<otus.size(); ++j){
      tmp=calc_otu_dist(otus[i],otus[j]);
      cout << "# "<<i<< " -- " << j << " : " << tmp << endl;
      if (tmp>0.9){
        cout << "WRONG: "<< i<< " -- " << j << " : " << tmp << endl;
        ++wrongCount;
      }
    }
  }
  for (i=0; i<otus.size(); ++i){
    tmp=calc_otu_intradist(otus[i]);
    cout << "# "<<i<< " : " << tmp << endl;
    if (tmp<0.9){
      cout << "WRONG: "<< i<< " : " << tmp << endl;
      ++wrongCount;
    }
  }
  cout << "# Wrong count: " << wrongCount << endl;

  return(0);
}

