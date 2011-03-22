#include <eutils/emain.h>
#include <eutils/efile.h>
#include <eutils/eheap.h>
#include <eutils/ethread.h>

#include "cluster-common.h"

/*
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

float cooc_dist2(int i,int j,const estrhash& arr,const eintarray& otuarr,estrhashof<earray<estr> >& samples,estrhash& accsample,estrhashof<int>& arrind)
{
  int k,l;
  estr acc;
  estr sample;

  acc=arr.keys(i);
  if (!accsample.exists(acc)) return(1.0);
  sample=accsample[acc];
  
  eintarray cooc1,cooc2;

  ldieif(!samples.exists(sample),"sample not found: "+sample);
  for (k=0; k<samples[sample].size(); ++k){
    if (acc==samples[sample][k]) continue;
    if (cooc1.find(otuarr[arrind[samples[sample][k]]])==-1)
      cooc1.add(otuarr[arrind[samples[sample][k]]]);
  }

  acc=arr.keys(j);
  if (!accsample.exists(acc)) return(1.0);
  sample=accsample[acc];
  ldieif(!samples.exists(sample),"sample not found: "+sample);
  for (k=0; k<samples[sample].size(); ++k){
    if (acc==samples[sample][k]) continue;
    if (cooc2.find(otuarr[arrind[samples[sample][k]]])==-1)
      cooc2.add(otuarr[arrind[samples[sample][k]]]);
  }


  int common=0;
  for (k=0; k<cooc1.size(); ++k){
    if (cooc2.find(cooc1[k])!=-1) ++common;
  }
 
//  cout << "# " << i << " " << j << " " << cooc1.size() << " " << cooc2.size() << " " << common << endl;

  return(1.0-(float)common/(cooc1.size()+cooc2.size()-common));
}

float cooc_dist3(int i,int j,const estrhash& arr,const eintarray& otuarr,estrhashof<earray<estr> >& samples,estrhash& accsample,estrhashof<int>& arrind,int otucount)
{
  int k,l;
  estr acc;
  estr sample;

  int cooc1,cooc2,common;
  char otu_neigh[otucount];

  acc=arr.keys(i);
  if (!accsample.exists(acc)) return(1.0);
  sample=accsample[acc];

  cooc1=0; cooc2=0; common=0;
  for (k=0; k<otucount; ++k)
    otu_neigh[k]=0;

  ldieif(!samples.exists(sample),"sample not found: "+sample);
  for (k=0; k<samples[sample].size(); ++k){
    if (acc==samples[sample][k]) continue;
    l=otuarr[arrind[samples[sample][k]]];
    if (otu_neigh[l]==0)
      { otu_neigh[l]=0x01; ++cooc1; }
  }

  acc=arr.keys(j);
  if (!accsample.exists(acc)) return(1.0);
  sample=accsample[acc];
  ldieif(!samples.exists(sample),"sample not found: "+sample);
  for (k=0; k<samples[sample].size(); ++k){
    if (acc==samples[sample][k]) continue;
    l=otuarr[arrind[samples[sample][k]]];
    if (otu_neigh[l]&0x10==0)
      { if (otu_neigh[l]==0x01) ++common; otu_neigh[l]=otu_neigh[l]|0x10; ++cooc2; }
  }

  return(1.0-(float)common/(cooc1+cooc2-common));
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
*/


int emain()
{ 
  estr ofile="cluster.dat";
  float t=0.90;
  epregister(t);
  epregister(ofile);
//  estr outfile="cooc_distances.dat";
//  epregister(outfile);
  eparseArgs(argvc,argv);

  ldieif(argvc<2,"syntax: "+efile(argv[0]).basename()+" <seqali>");

  int i;
  estrarray arr;

  load_seqs(argv[1],arr);
  
  earray<eseqdist> mindists;

//  for (i=0; i<10; ++i)
  calc_dists_nogap(arr,mindists,0,1,0.0);
//  calc_dists2(arr,mindists,0,1,0.9);
  cout << "# distances within threshold: "<<mindists.size() << endl;

  eseqcluster cluster;
  cluster.init(arr.size());

  for (i=mindists.size()-1; i>=0; --i)
    cluster.add(mindists[i]);

//  cluster.save(ofile,arr);
//  cout << "# done writing cluster.dat" << endl;

  

/*
  estr line;
  estr name;
  efile f(argv[1]);
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
    }else
      arr[name]+=line;
  }
  cout << "# seqs: " << arr.size() << endl;
*/

/*
  estrhashof<earray<estr> > samples;
  estrhash accsample;

  efile f2(argv[2]);

  f2.readln(line);
  i=line.find("--");
  while (i!=-1){
    name=line.substr(i+3);
    samples.add(name,earray<estr>());
    do {
      if (!f2.readln(line)) { i=-1; break; }
      i=line.find("--");
      if (i==-1 && arr.exists(line)){ samples[name].add(line); accsample.add(line,name); }
    } while(i==-1);
  }  
  cout << "# samples: " << samples.size() << endl;
*/

  int j,i2;
//  float dist_mat[arr.size()*arr.size()/2];

  ebasicarray<float> dist_mat;
//  ebasicarray<float> cooc_dist_mat;

//  for (i=0; i<arr.size()-1; ++i)
//    for (j=i+1; j<arr.size(); ++j)
//      dist_mat.add(dist(arr.values(i),arr.values(j)));

/*
  dist_mat.reserve(arr.size()*(arr.size()+1)/2-arr.size());
  for (i=0; i<arr.size()*(arr.size()+1)/2-arr.size(); ++i){
    dist_mat.add(0.0);
//    cooc_dist_mat.add(0.0);
  }
*/

/*
  int c=0;
  int l;
  for (i=0; i<arr.size()/2; ++i){
    getL(c,arr.size(),i,j);
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
*/

/*
  ethread thread[2];
  thread[0].wait();
  thread[1].wait();
  evararray vararr;

  vararr.add(new int(0));
  vararr.add(new int(arr.size()/6));
  vararr.add(dist_mat);
  vararr.add(arr);
  thread[0].run(efunc(calc,vararr));

  vararr.add(new int(arr.size()/6));
  vararr.add(new int(arr.size()*2/6));
  vararr.add(dist_mat);
  vararr.add(arr);
  thread[1].run(efunc(calc,vararr));
  calc(arr.size()*2/6,arr.size()/2,dist_mat,arr);
*/






/*
  efile ofile("cluster.dat");
  eintarray dsort;
  earray<eintarray> otus;
  eintarray otuarr;
  estr tmpstr;

  if (!ofile.exists()){
    cout << "# calculating otus" << endl;
    calc(0,arr.size()/2,dist_mat,arr);
//  calc(0,arr.size()/4,dist_mat,arr);
//  calc2(dist_mat,arr);

    cout << "# distances: " << dist_mat.size() << endl;

//      dist_mat[i*arr.size()-2*i+j]+=dist(arr.values(i),arr.values(j));

//    dsort=eintarray(sort(dist_mat));
//    cluster_complete(arr,dist_mat,dsort,otus,otuarr,t);

    cout << "# writing otus" << endl;
    dist_mat.serial(tmpstr);
//    otus.serial(tmpstr);
//    otuarr.serial(tmpstr);
    ofile.write(tmpstr);

    cout << "# otus: " << otus.size() << endl;
  }else{
    cout << "# loading otus" << endl;
    ofile.read(tmpstr);
    i=0;
    i=dist_mat.unserial(tmpstr,i);
    cout << "# dist_mat.size(): " << dist_mat.size() << endl;
//    i=otus.unserial(tmpstr,i);
//    cout << "# otus.size(): " << otus.size() << endl;
//    i=otuarr.unserial(tmpstr,i);
//    cout << "# otuarr.size(): " << otuarr.size() << endl;
  }
  ofile.close();
*/

/*
  int otucount=-1;
  for (i=0; i<otuarr.size(); ++i){
    if (otuarr[i]>otucount) otucount=otuarr[i];
  }
  otucount+=1;
  cout << "# otucount: " << otucount << endl;
*/

//  otuarr.clear(); otus.clear();
//  dsort=eintarray(sort(dist_mat));
  
//  earray<eintarray> otus;
//  cluster_complete2(arr,dist_mat,dsort,otus,otuarr,0.90);

  


/*
  earray<eintarray> neigharr;
  cooc_init(neigharr,arr,otuarr,samples,accsample,arrind,otucount);

 
//  cooc_calc(0,arr.size()/2,cooc_dist_mat,neigharr);
  cout << "# Calculating cooc distances" << endl;
  cooc_calc(0,arr.size()/2,cooc_dist_mat,neigharr);

  cout << "# Writing cooc distances to file" << endl;
  efile of(outfile);
  for (i=0; i<cooc_dist_mat.size(); ++i)
    of.write(estr(cooc_dist_mat[i])+" "+estr(dist_mat[i])+"\n");
  of.close();
*/


/*
  efile of(outfile);
  int otucount=0;
  for (i=0; i<otus.size(); ++i){
    if (otus[i].size()==0) continue;
    of.write(">OTU"+estr(otucount)+" "+estr(otus[i].size())+"\n");
    for (j=0; j<otus[i].size(); ++j)
      of.write(arr.keys(otus[i][j])+"\n");
    ++otucount;
  }
*/

  return(0);
}

