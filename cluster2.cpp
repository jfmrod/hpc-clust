#include <eutils/emain.h>
#include <eutils/efile.h>
#include <eutils/eheap.h>
#include <eutils/ethread.h>
#include <math.h>

inline float dist(const estr& s1,const estr& s2)
{
  int i;
  int count=0;
  for (i=0; i<s1.len(); ++i){
    if (s1[i]==s2[i]) ++count;
  }
  return((float)count/(float)s1.len());
}

inline void getL2(int& l,int w,int x,int y)
{
  if (y<x) { int t; t=x; x=y; y=t; }

  --w;
  if (x>w/2) { x=w-x; y-=1; l=x*w+y; return; }

  y-=x+1;
  l=x*w+y;
  return;
}

inline void getL(int& l,int w,int x,int y)
{
  --w;
  if (x>w/2) { x=w-x; y-=1; l=x*w+y; return; }

  y-=x+1;
  l=x*w+y;
  return;
}

inline void getXY(int l,int w,int& x,int& y)
{
  --w;
  x=(l/w);
  y=(l%w);
  if (y<w-x){ y+=x+1; return; }
  y+=1;
  x=w-x;
}

void calc2(ebasicarray<float>& dist_mat,const estrhash& arr)
{
  int c=0;
  int i,i2,j;
  for (i=0; i<arr.size()/2; ++i){
    for (j=i+1; j<arr.size(); ++j)
      dist_mat[c++]=dist(arr.values(i),arr.values(j));
    i2=arr.size()-i-1;
    for (j=i2+1; j<arr.size(); ++j)
      dist_mat[c++]=dist(arr.values(i2),arr.values(j));
  }
  if (arr.size()%2==1){
    i=arr.size()/2;
    for (j=i+1; j<arr.size(); ++j)
      dist_mat[c++]=dist(arr.values(i),arr.values(j));
  }
  cout << "# c: " << c << endl;
}

float cooc_dist(int i,int j,earray<eintarray>& neigharr)
{
  int k;

  int cooc1,cooc2,common;

  cooc1=0; cooc2=0; common=0;
  if (neigharr[i].size()==0 || neigharr[j].size()==0) return(1.0);
  ldieif(j>=neigharr.size(),"j larger than neigharr.size()? "+estr(j)+">"+estr(neigharr.size()));
  ldieif(i>=neigharr.size(),"i larger than neigharr.size()? "+estr(i)+">"+estr(neigharr.size()));
  ldieif(neigharr[i].size() != neigharr[j].size(),"size mismatch: "+estr(neigharr[i].size())+" != "+estr(neigharr[j].size()));

  for (k=0; k<neigharr[i].size(); ++k){
    if (neigharr[i][k]!=0){ ++cooc1; if (neigharr[j][k]!=0) ++common; }
    if (neigharr[j][k]!=0){ ++cooc2; }
  }
  return(1.0-(float)common/(cooc1+cooc2-common));
}

void cooc_init(earray<eintarray>& neigharr,const estrhash& arr,const eintarray& otuarr,estrhashof<earray<estr> >& samples,estrhash& accsample,estrhashof<int>& arrind,int otucount)
{
 
  int i,k,l;
  estr acc;
  estr sample;

  cout << "# otucount: " << otucount << endl;


  for (i=0; i<arr.size(); ++i)
    neigharr.add(eintarray());

  cout << "# neighcount: " << neigharr.size() << endl;
  for (i=0; i<arr.size(); ++i){
    acc=arr.keys(i);
    if (!accsample.exists(acc)) continue;
    sample=accsample[acc];

    for (k=0; k<otucount; ++k)
      neigharr[i].add(0);

    ldieif(!samples.exists(sample),"sample not found: "+sample);
    for (k=0; k<samples[sample].size(); ++k){
      if (acc==samples[sample][k]) continue;
      l=otuarr[arrind[samples[sample][k]]];
      ldieif(l<0 || l>=otucount,"Something wrong");
//      cout << " i: " << i << " l: " << l << endl;
      ++neigharr[i][l];
    }
  }

  cout << "# neigharr.size(): " << neigharr.size() << endl;
}

void cooc_calc(int start,int end,ebasicarray<float>& dist_mat,earray<eintarray>& neigharr)
{
  int c=0;
  int i,i2,j;

  for (i=start; i<end; ++i){
    getL(c,neigharr.size(),i,i+1);
    for (j=i+1; j<neigharr.size(); ++j)
      dist_mat[c++]=cooc_dist(i,j,neigharr);
    i2=neigharr.size()-i-1;
    for (j=i2+1; j<neigharr.size(); ++j)
      dist_mat[c++]=cooc_dist(i2,j,neigharr);
  }
  if (end==neigharr.size()/2 && neigharr.size()%2==1){
    getL(c,neigharr.size(),i,i+1);
    i=neigharr.size()/2;
    for (j=i+1; j<neigharr.size(); ++j)
      dist_mat[c++]=cooc_dist(i,j,neigharr);
  }
}

void calc(int start,int end,ebasicarray<float>& dist_mat,const estrhash& arr)
{
  int c=0;
  int i,i2,j;
  for (i=start; i<end; ++i){
    getL(c,arr.size(),i,i+1);
    for (j=i+1; j<arr.size(); ++j)
      dist_mat[c++]=dist(arr.values(i),arr.values(j));
    i2=arr.size()-i-1;
    for (j=i2+1; j<arr.size(); ++j)
      dist_mat[c++]=dist(arr.values(i2),arr.values(j));
  }
  if (end==arr.size()/2 && arr.size()%2==1){
    getL(c,arr.size(),i,i+1);
    i=arr.size()/2;
    for (j=i+1; j<arr.size(); ++j)
      dist_mat[c++]=dist(arr.values(i),arr.values(j));
  }
}

void cluster_complete(const estrhash& arr,const ebasicarray<float>& dist_mat,const eintarray& dsort,earray<eintarray>& otus,eintarray& otuarr,float t)
{
  int x,y;
  int arr1,arr2;
  int i,j,l,k;
  bool complete;

  otuarr.clear();
  for (i=0; i<arr.size(); ++i)
    otuarr.add(-1);

  for (i=dist_mat.size()-1; i>0; --i){
    getXY(dsort[i],arr.size(),x,y);
    if (dist_mat[dsort[i]]<t) break;
    if (otuarr[x]==otuarr[y] && otuarr[x]!=-1) continue;

    if (otuarr[x]==-1 && otuarr[y]==-1){
      otuarr[x]=otus.size(); otuarr[y]=otus.size();
      otus.add(eintarray());
      otus[otuarr[x]].add(x);
      otus[otuarr[y]].add(y);
    }else if (otuarr[x]==-1){
      complete=true;
      for (j=0; j<otus[otuarr[y]].size(); ++j){
        getL2(l,arr.size(),x,otus[otuarr[y]][j]);
        if (dist_mat[l]<t){ complete=false; break; }
      }
      if (!complete) continue;
      otuarr[x]=otuarr[y];
      otus[otuarr[x]].add(x);
    }else if (otuarr[y]==-1){
      complete=true;
      for (j=0; j<otus[otuarr[x]].size(); ++j){
        getL2(l,arr.size(),y,otus[otuarr[x]][j]);
        if (dist_mat[l]<t){ complete=false; break; }
      }
      if (!complete) continue;
      otuarr[y]=otuarr[x];
      otus[otuarr[y]].add(y);
    }else{
      complete=true;
      for (j=0; j<otus[otuarr[x]].size(); ++j){
        for (k=0; k<otus[otuarr[y]].size(); ++k){
          getL2(l,arr.size(),otus[otuarr[y]][k],otus[otuarr[x]][j]);
          if (dist_mat[l]<t){ complete=false; break; }
        }
        if (!complete) break;
      }
      if (!complete) continue;
      
      arr1=otuarr[x]; arr2=otuarr[y];
      if (arr1>arr2) { arr1=otuarr[y]; arr2=otuarr[x]; }

      for (j=0; j<otus[arr2].size(); ++j)
        otuarr[otus[arr2][j]]=arr1;
      otus[arr1]+=otus[arr2];
      otus[arr1].clear();
    }
//    cout << dsort[i] << " " << x << " " << seq1[dsort[i]] << " " << y << " " << seq2[dsort[i]] << " " << dist_mat[dsort[i]] << endl;
  }
  cout << "# i: " << dist_mat.size()-i << " ("<<1.0-(float)i/dist_mat.size()<<")" << endl;

  cout << "# tmp total otus: " << otus.size() << endl;

  for (i=0; i<otus.size(); ++i)
    if (otus[i].size()==0)
      { otus.erase(i); --i; }

  for (i=0; i<otuarr.size(); ++i){
    if (otuarr[i]==-1){
      otus.add(eintarray());
      otus[otus.size()-1].add(i);
      otuarr[i]=otus.size()-1;
    }
  }

  cout << "# total otus: " << otus.size() << endl;
}

void cluster_single(const estrhash& arr,const ebasicarray<float>& dist_mat,const eintarray& dsort,earray<eintarray>& otus,float t)
{
  int x,y;
  int arr1,arr2;
  int i,j;

  eintarray otuarr;
  for (i=0; i<arr.size(); ++i)
    otuarr.add(-1);

  for (i=dist_mat.size()-1; i>0; --i){
    getXY(dsort[i],arr.size(),x,y);
    if (dist_mat[dsort[i]]<t) break;
    if (otuarr[x]==otuarr[y] && otuarr[x]!=-1) continue;

    if (otuarr[x]==-1 && otuarr[y]==-1){
      otuarr[x]=otus.size(); otuarr[y]=otus.size();
      otus.add(eintarray());
      otus[otuarr[x]].add(x);
      otus[otuarr[y]].add(y);
    }else if (otuarr[x]==-1){
      otuarr[x]=otuarr[y];
      otus[otuarr[x]].add(x);
    }else if (otuarr[y]==-1){
      otuarr[y]=otuarr[x];
      otus[otuarr[y]].add(y);
    }else{
      arr1=otuarr[x]; arr2=otuarr[y];
      if (arr1>arr2) { arr1=otuarr[y]; arr2=otuarr[x]; }

      for (j=0; j<otus[arr2].size(); ++j)
        otuarr[otus[arr2][j]]=arr1;
      otus[arr1]+=otus[arr2];
      otus[arr1].clear();
    }
//    cout << dsort[i] << " " << x << " " << seq1[dsort[i]] << " " << y << " " << seq2[dsort[i]] << " " << dist_mat[dsort[i]] << endl;
  }
  cout << "# i: " << dist_mat.size()-i << " ("<<1.0-(float)i/dist_mat.size()<<")" << endl;

  cout << "# tmp total otus: " << otus.size() << endl;

  for (i=0; i<otus.size(); ++i)
    if (otus[i].size()==0)
      { otus.erase(i); --i; }

  for (i=0; i<otuarr.size(); ++i){
    if (otuarr[i]==-1){
      otus.add(eintarray());
      otus[otus.size()-1].add(i);
      otuarr[i]=otus.size()-1;
    }
  }

  cout << "# total otus: " << otus.size() << endl;
}
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

