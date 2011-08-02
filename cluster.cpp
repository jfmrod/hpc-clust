#include <eutils/emain.h>
#include <eutils/efile.h>
#include <eutils/eheap.h>
#include <eutils/etime.h>
#include <eutils/etimer.h>
#include <eutils/eoption.h>

#include "cluster-common.h"
#include "eseqclusteravg.h"

//eseqcluster cluster;

eblockarray<eseqdist> dists;

eseqclusteravg avgcluster; // avg linkage
eseqcluster clcluster; // complete linkage
eseqclustersingle slcluster; // single linkage

//eblockarray<eseqdist> mindists;
//ebasicarray<eseqdist> mindists;
int partsFinished=0;
int partsTotal=100;
estrarray arr;
unsigned totaldists;

int seqlen=0;

/*
void p_calc_dists_nogap(int node,int tnodes,float thres)
{
  eblockarray<eseqdist> dists;
//  ebasicarray<eseqdist> dists;
  calc_dists_nogap_compressed(arr,dists,seqlen,node,tnodes,thres);

  mutexDists.lock();
  ++partsFinished;
//  mindists+=dists;
  cluster.dists.merge(dists);
  mutexDists.unlock();
}
*/

estr args2str(int argvc,char **argv)
{
  estr tmpstr;
  int i;
  for (i=0; i<argvc; ++i)
    tmpstr+=estr(argv[i])+" ";
  tmpstr.del(-1);
  return(tmpstr);
}

emutex mutex;

/*
void gap_calc_dists(estrarray& arr,eblockarray<eseqdist>& dists,int seqlen,int node,int tnodes,float thres)
{
  eblockarray<eseqdist> tmpdists;
  t_calc_dists2<estrarray,eseqdist,eblockarray<eseqdist>,dist_compressed>(arr,tmpdists,seqlen,node,tnodes,thres);
  mutex.lock();
  dists+=tmpdists;
  mutex.unlock();
}

void gapnoise_calc_dists(estrarray& arr,eblockarray<eseqdist>& dists,int seqlen,int node,int tnodes,float thres)
{
  eblockarray<eseqdist> tmpdists;
  t_calc_dists_noise2<estrarray,eseqdist,eblockarray<eseqdist>,dist_compressed>(arr,tmpdists,seqlen,node,tnodes,thres);
  mutex.lock();
  dists+=tmpdists;
  mutex.unlock();
}
*/

int emain()
{ 
  cout << "# " << date() << endl;
  cout << "# " << args2str(argvc,argv) << endl;
  float avgmutseq=0.0;
  epregister(avgmutseq);

  eoption<efunc> dfunc;

  dfunc.choice=0;
//  dfunc.add("gap",gap_calc_dists);
//  dfunc.add("gap+noise",gapnoise_calc_dists);
  dfunc.add("gap",t_calc_dists<estrarray,eseqdist,eblockarray<eseqdist>,dist_compressed>);
  dfunc.add("nogap",t_calc_dists<estrarray,eseqdist,eblockarray<eseqdist>,dist_nogap_compressed>);
  dfunc.add("tamura",t_calc_dists<estrarray,eseqdist,eblockarray<eseqdist>,dist_tamura_compressed>);
  dfunc.add("gap+noise",t_calc_dists_noise<estrarray,eseqdist,eblockarray<eseqdist>,dist_compressed>);
  dfunc.add("nogap+noise",t_calc_dists_noise<estrarray,eseqdist,eblockarray<eseqdist>,dist_nogap_compressed>);
  dfunc.add("tamura+noise",t_calc_dists_noise<estrarray,eseqdist,eblockarray<eseqdist>,dist_tamura_compressed>);

  epregisterClass(eoption<efunc>);
  epregisterClassMethod2(eoption<efunc>,operator=,int,(const estr& val));

  epregister(dfunc);

  estr ofile="cluster-results";
  estr dfile;
  float t=0.90;
  int ncpus=1;
  epregister(t);
  epregister(ncpus);
  epregister(ofile);
  epregister(dfile);
//  estr outfile="cooc_distances.dat";
//  epregister(outfile);
  eparseArgs(argvc,argv);

  cout << "# distance function: " << dfunc.key() << endl;

  epregisterClass(eseqdist);
  epregisterClassSerializeMethod(eseqdist);
  epregisterClassProperty(eseqdist,dist);
  epregisterClassProperty(eseqdist,x);
  epregisterClassProperty(eseqdist,y);

  epregisterClass(ebasicarray<eseqdist>);
  epregisterClassInheritance(ebasicarray<eseqdist>,ebasearray);
  epregisterClassMethod(ebasicarray<eseqdist>,subset);
  epregisterClassSerializeMethod(ebasicarray<eseqdist>);


  ldieif(argvc<2,"syntax: "+efile(argv[0]).basename()+" <seqali>");

  int i;
  if (avgmutseq>0.0)
    load_seqs_mutate_compressed(argv[1],arr,seqlen,avgmutseq);
  else
    load_seqs_compressed(argv[1],arr,seqlen);
  
  float dtime,stime;
  etimer t1;
  t1.reset();

  etaskman taskman;

//  if (ncpus==1) partsTotal=1;

  efile df(dfile);
  if (dfile.len()==0 || !df.exists()){
    cout << "# computing distances" << endl;
    if (partsTotal>(arr.size()-1)*arr.size()/20) partsTotal=(arr.size()-1)*arr.size()/20;
    for (i=0; i<partsTotal; ++i){
      
//      taskman.addTask(dfunc.value(),evararray(arr,dists,(const int&)seqlen,(const int&)i,(const int&)partsTotal,(const float&)t));
      taskman.addTask(dfunc.value(),evararray(mutex,arr,dists,(const int&)seqlen,(const int&)i,(const int&)partsTotal,(const float&)t));
    }
//      taskman.addTask(efunc(cluster,&eseqclusterCount::calc),evararray(arr,(const int&)seqlen,(const int&)i,(const int&)partsTotal,(const float&)t));
//      taskman.addTask(p_calc_dists_nogap,evararray((const int&)i,partsTotal,t));

    taskman.createThread(ncpus);
    cout << "# finished creating threads: "<<ncpus << endl;
    taskman.wait();
    cout << "# all tasks finished" << endl;

    dtime=t1.lap()*0.001;
    cout << "# time calculating distances: " << dtime << endl;
    cout << "# distances within threshold: " << dists.size() << endl;

//    heapsort(mindists);
    dists.sort();
    stime=t1.lap()*0.001;

/*
    if (dfile.len()){
      cout << "# saving distances to file: "<<dfile << endl;
      estr str;
      mindists.serial(str);
      df.write(str);
      df.close();
    }
*/
  }else{


    cout << "# loading distances from file: "<<dfile << endl;

/*
    estr str;
    df.read(str);
    ldieif(mindists.unserial(str,0)==-1,"problem loading distance file: "+dfile);
    df.close();
*/
  } 

  totaldists=dists.size();
  cout << "# time sorting distances: " << stime << endl;

  cout << "# initializing cluster"<<endl;
  clcluster.init(arr.size(),ofile+".cl.dat");
  slcluster.init(arr.size(),ofile+".sl.dat");
  avgcluster.init(arr.size(),ofile+".avg.dat");

  cout << "# starting clustering"<<endl;
  t1.reset();
  int tmp;
  int lastupdate=0;
  for (i=dists.size()-1; i>=0; --i){
    avgcluster.add(dists[i]);
    clcluster.add(dists[i]);
    slcluster.add(dists[i]);
//    cluster.update(i-1);
//    if (cluster.mergecount%1000==0 && cluster.mergecount!=lastupdate) { lastupdate=cluster.mergecount; cout << "# merged " << cluster.update(i-1) << " seqs" << endl; }
//    if (i%10000==0) { cout << i/10000 << " "<< mindists[i].dist << " " << arr.size()-cluster.mergecount << " " << cluster.smatrix.size() << endl; }
//    if (i%10000==0) { tmp=cluster.update(mindists,i-1); cout << i/10000 << " "<< mindists[i].dist << " " << arr.size()-cluster.mergecount << " " << cluster.smatrix.size() << " " << tmp << endl; }
  }
  float clustime=t1.lap()*0.001;
  cout << "# time calculating distances: " << dtime << endl;
  cout << "# time sorting distances: " << stime << endl;
  cout << "# time clustering: " << clustime << endl;
  cout << "# total time: " << dtime+clustime+stime << endl;
  cout << "# distances within threshold: " << totaldists << endl;

  clcluster.save(ofile+".cl.otu",arr);
  cout << "# done writing complete linkage clustering to: "<<ofile+".cl" << endl;
  slcluster.save(ofile+".sl.otu",arr);
  cout << "# done writing single linkage clustering to: "<<ofile+".sl" << endl;
  avgcluster.save(ofile+".avg.otu",arr);
  cout << "# done writing average linkage clustering to: "<<ofile+".avg" << endl;


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

