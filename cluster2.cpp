#include <eutils/emain.h>
#include <eutils/efile.h>
#include <eutils/eheap.h>
#include <eutils/ethread.h>
#include <math.h>

#include "cluster-common.h"

void calc_min(const estrhash& arr,earray<eintarray>& refarr,float cthres,float rthres)
{
  int count=0;
  int mincount=0;
  int i,j,k,l;
  float tmpid;
  for (i=0; i<refarr.size(); ++i){
    for (j=0; j<refarr[i].size(); ++j){
      for (k=j+1; k<refarr[i].size(); ++k){
        tmpid=dist(arr.values(refarr[i][j]),arr.values(refarr[i][k]));
        if (tmpid>cthres) ++mincount;
        ++count;
      }
    }
  }
  for (i=0; i<refarr.size(); ++i){
    for (j=i+1; j<refarr.size(); ++j){
      tmpid=dist(arr.values(refarr[i][0]),arr.values(refarr[j][0]));
      if (tmpid < cthres+2.0*rthres-2.0) continue;

      for (k=1; k<refarr[i].size(); ++k){
        tmpid=dist(arr.values(refarr[i][k]),arr.values(refarr[j][0]));
        if (tmpid>cthres) ++mincount;
        if (tmpid < cthres+rthres-1.0) continue;
        for (l=1; l<refarr[j].size(); ++l){
//          if (tmpid < 2.0*thres-1.0) continue;
          tmpid=dist(arr.values(refarr[i][k]),arr.values(refarr[j][0]));
          if (tmpid>cthres) ++mincount;
          ++count;
        }
      }
    }
  }
  cout << "# total computed distances: " << count << endl;
  cout << "# total distances less than thres: " << mincount << endl;
}

void ref_init(const estrhash& arr,earray<eintarray>& refarr,float rthres)
{
  int i,j;
  refarr.add(eintarray());
  refarr[0].add(0);
  for (i=1; i<arr.size(); ++i){
    int minref=-1;
    float minid=0.0;
    float tmpid;
    for (j=0; j<refarr.size(); ++j){
      tmpid=dist(arr.values(refarr[j][0]),arr.values(i));
      if (tmpid>minid) { minid=tmpid; minref=j; }
    }
    if (minid<rthres) {
      refarr.add(eintarray());
      refarr[refarr.size()-1].add(i);
//      cout << "# refcount: " << refarr.size() << " i: " << i << " minid: " << minid << endl;
    } else
      refarr[minref].add(i);
  }
/*
  for (i=0; i<refarr.size(); ++i){
    cout << i << " " << refarr[i].size() << endl;
  }
*/
}

void ref_stats(estrhash& arr,earray<eintarray>& refarr,float thres)
{
  int i,j,k;
  for (i=0; i<refarr.size(); ++i){
    float avgd=0.0;
    float avgd2=0.0;
    float min=1.0;
    float max=0.0;
    float tmpd;
    int count=0;
    for (j=0; j<refarr[i].size(); ++j){
      for (k=j+1; k<refarr[i].size(); ++k){
        tmpd=dist(arr.values(refarr[i][j]),arr.values(refarr[i][k]));
        if (min>tmpd) min=tmpd; if (max<tmpd) max=tmpd;
        avgd+=tmpd;
        avgd2+=tmpd*tmpd;
        ++count;
      }
    }
    int count2=(refarr[i].size()*(refarr[i].size()+1)/2-refarr[i].size());
    avgd=avgd/count;
    avgd2=sqrt(avgd2/count-avgd*avgd);
    if (min>thres) cout << "* ";
    cout << i << " " << refarr[i].size() << " " << avgd << " " << avgd2 << " " << min << " " << max << " "<<count << " " << count2 << endl;
  }
}


int emain()
{
  float ct=0.80;
  float rt=0.95;
  epregister(ct);
  epregister(rt);

  eparseArgs(argvc,argv);

  ldieif(argvc<2,"syntax: "+efile(argv[0]).basename()+" <seqali>");


  estr line;
  estr name;
  efile f(argv[1]);

  int i;
  estrhash arr;
  estrhashof<int> arrind;
 
  while (f.readln(line)){
    if (line.len()==0 || line[0]=='#') continue;
    
    i=line.find(" ");
    if (i==-1) continue;
    name=line.substr(0,i);
    line.del(0,i);
    line.trim();

    i=name.find("|");
    ldieif(i==-1,"| not found in line");
    name.del(i);

    if (!arr.exists(name)){
      arrind.add(name,arr.size());
      arr.add(name,line);
      arr[name].reserve(1500);
    }else
      arr[name]+=line;
  }
  cout << "# seqs: " << arr.size() << endl;

/*
  ebasicarray<float> dist_mat;
  for (i=0; i<arr.size()*(arr.size()+1)/2-arr.size(); ++i)
    dist_mat.add(0.0);

  calc(0,arr.size()/2,dist_mat,arr);

  int count=0;
  for (i=0; i<dist_mat.size(); ++i)
    if (ct < dist_mat[i]) ++count;

  cout << "# total distances: " << dist_mat.size() << endl;
  cout << "# total min distances: " << count << endl;
*/

//  float rthres=0.5+t*0.5;
  earray<eintarray> refarr;
  ref_init(arr,refarr,rt);
//  ref_stats(arr,refarr,t);
//  calc_min(arr,refarr,ct,rt);

  cout << "# reference sequences: " << refarr.size() << endl;

  return(0);
}

