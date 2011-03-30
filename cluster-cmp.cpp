#include <eutils/emain.h>
#include <eutils/efile.h>
#include <eutils/eheap.h>
#include <eutils/esystem.h>

#include "cluster-common.h"


estrhashof<int> arrind;
eintarray otuarr,otuarr2;
int otucount,otucount2;

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

int emain()
{
  eparseArgs(argvc,argv);

  ldieif(argvc<4,"syntax: "+efile(argv[0]).basename()+" <seqs> <clusters1> <clusters2>");

  load_seqs(argv[1],arrind);
  load_clusters(argv[2],arrind,otuarr,otucount);
  load_clusters(argv[3],arrind,otuarr2,otucount2);

  earray<eintarray> otuvec,otuvec2;
  cluster_init(otuvec,arrind,otuarr,otucount);
  cluster_init(otuvec2,arrind,otuarr2,otucount2);

  int i,j,jmax;
  float tmp;
  float max;
  int matchcount=0;
  int size1,size2;
  for (i=0; i<otuvec.size(); ++i){
    max=0.0;
    jmax=-1;
    for (j=0; j<otuvec2.size(); ++j){
      tmp=otuvec_dist(otuvec[i],otuvec2[j]);
//      cout << "# "<<i<< " -- " << j << " : " << tmp << endl;
      if (tmp>max) { max=tmp; jmax=j; }
    }
    cout << i << " --- " << jmax << " : "<< otuSize(otuvec[i])<< " " << otuSize(otuvec2[jmax])<< " "<< (otuSize(otuvec[i])+otuSize(otuvec2[jmax]))/2.0 << " " << max << endl;
    if (max>0.95)
      ++matchcount;
  }
  cout << "# Matched count: " << matchcount << endl;
  cout << "# matched: " << matchcount*100.0/(float)otuvec.size() << endl;

  return(0);
}

