#include <eutils/emain.h>
#include <eutils/efile.h>
#include <eutils/eheap.h>
#include <eutils/edcserver.h>
#include <eutils/esystem.h>
#include <eutils/etimer.h>

#include "cluster-common.h"

//estrhash arr;
estrarray arr;
ebasicarray<eseqdist> mindists;
estr ofile="cluster.dat";
etimer t1;


/*
void load_seqs(const estr& filename,estrhash& arr)
{
  estr line;
  estr name;
  efile f(filename);

  int i;

  while (f.readln(line)){
    if (line.len()==0 || line[0]=='#') continue;
    
    i=line.find(" ");
    if (i==-1) continue;
    name=line.substr(0,i);
    line.del(0,i);
    line.trim();
    if (!arr.exists(name)){
      arr.add(name,line);
      arr[name].reserve(1500);
    }else
      arr[name]+=line;
  }
  cout << "# seqs: " << arr.size() << endl;
}
*/
void getDistances(edcserver& server);
void serverCluster(edcserver& server);
void serverDistanceThreshold(edcserver& server);
void serverMergeResults(edcserver& server);

void startComputation(edcserver& server)
{
  t1.reset();
  cout << "# starting distributed computation" << endl;

  int i;
  server.onAllReady=getDistances;
  for (i=0; i<server.sockets.size(); ++i)
    server.getClient(i).call("nodeComputeDistances",evararray(i,server.sockets.size(),0.9));
}

int step=1;
long int totaldists=0;
long int itotaldists=0;
eintarray cdists;
eseqcluster cluster;
int tbucket=100000;

ebasicarray<float> cmins;

void getDistances(edcserver& server)
{
  int i;
  for (i=0; i<server.sockets.size(); ++i){
//    ldieif(server.getClient(i).result.isNull(),"not supposed to happen");
    if (server.getClient(i).result.isNull()) continue;
    cdists.add(server.getClient(i).result.get<int>());
    cmins.add(1.0);
    totaldists+=server.getClient(i).result.get<int>();
  }
  itotaldists=totaldists;
  cout << "# total distances under threshold: " << totaldists << endl;
  cout << "# time calculating distance: "<< t1.lap() <<endl;

  server.onAllReady=serverDistanceThreshold;
  cout << "# getting distances" << endl;
  for (i=0; i<server.sockets.size(); ++i){
    if (cdists[i]<=0) continue;
    server.getClient(i).call("nodeGetCount",evararray(tbucket));
  }
}

float mindist;
float mdist;
long int totalmergedists=0;

void serverMergeResults(edcserver& server)
{
  int i;
  int tmpi;
  for (i=0; i<server.sockets.size(); ++i){
    if (server.getClient(i).result.isNull()) continue;

    tmpi=server.getClient(i).result.get<int>();
//    cout << "# client: "<< i << " merged distances: "<< tmpi << endl;
    cdists[i]-=tmpi;
    totaldists-=tmpi;
    totalmergedists+=tmpi;
    server.getClient(i).result.clear();
  }

  server.onAllReady=serverDistanceThreshold;
  cout << "# getting next "<<tbucket<<" seqs from each node: "<<step<<" remaining dists: " << totaldists<< endl;
  for (i=0; i<server.sockets.size(); ++i){
    if (cdists[i]<=0) continue;
    server.getClient(i).call("nodeGetCount",evararray(tbucket));
  }
}

void serverDistanceThreshold(edcserver& server)
{
  int i;
  int count=0;
  mindist=0.0;
  mdist=1.0;
  for (i=0; i<server.sockets.size(); ++i){
    if (server.getClient(i).result.isNull()){ if (mindist<cmins[i]) mindist=cmins[i]; if (mdist>cmins[i]) mdist=cmins[i]; continue; }

    ebasicarray<eseqdist> *tmparr;

    tmparr=&server.getClient(i).result.get<ebasicarray<eseqdist> >();
    mindists+=*tmparr;
    if (tmparr->size()>0){
//      cout << "# client: "<<i << " min: " << tmparr->at(0).dist << " max: " << tmparr->at(tmparr->size()-1).dist << " cdists: " << cdists[i] << " size: "<< tmparr->size() << endl;
      cmins[i]=tmparr->at(0).dist;
      cdists[i]-=tmparr->size();
      totaldists-=tmparr->size();
      if (mindist<cmins[i]) mindist=cmins[i];
      if (mdist>cmins[i]) mdist=cmins[i];
    }
//    else
//      cout << "# client: "<<i<<" empty array"<<endl; 
    server.getClient(i).result.clear();
  }

  server.onAllReady=serverCluster;
  cout << "# getting sequences under threshold: "<<mdist <<" remaining dists: " << totaldists<< endl;

  bool nomoredistances=true;
  for (i=0; i<server.sockets.size(); ++i){
    if (cdists[i]<=0) continue;
    server.getClient(i).call("nodeGetThres",evararray(mdist));
    nomoredistances=false;
  }
  if (nomoredistances)
    serverCluster(server);
}

void serverCluster(edcserver& server)
{
  int i;
  int count=0;
  mindist=0.0;
  mdist=1.0;
  for (i=0; i<server.sockets.size(); ++i){
    if (server.getClient(i).result.isNull()){ if (mindist<cmins[i]) mindist=cmins[i]; if (mdist>cmins[i]) mdist=cmins[i]; continue; }

    ebasicarray<eseqdist> *tmparr;

    tmparr=&server.getClient(i).result.get<ebasicarray<eseqdist> >();
    mindists+=*tmparr;
    if (tmparr->size()>0){
//      cout << "# client: "<<i << " min: " << tmparr->at(0).dist << " max: " << tmparr->at(tmparr->size()-1).dist << " cdists: " << cdists[i] << " size: "<< tmparr->size() << endl;
      cmins[i]=tmparr->at(0).dist;
      cdists[i]-=tmparr->size();
      totaldists-=tmparr->size();
      if (mindist<cmins[i]) mindist=cmins[i];
      if (mdist>cmins[i]) mdist=cmins[i];
    }
//    else
//      cout << "# client: "<<i<<" empty array"<<endl; 
    server.getClient(i).result.clear();
  }

  cout << "# Sorting array with "<<mindists.size()<< " distances" << endl;
  heapsort(mindists);

  cout << "# min: " << mindists[0].dist << " minall: "<< mindist << " max: "<<mindists[mindists.size()-1].dist << endl;

//  for (i=mindists.size()-1; i>=0 && (mindists[i].dist>=mindist || totaldists<=0); --i){
  for (i=mindists.size()-1; i>=0; --i){
    ldieif(mindists[i].count==0,"not right");
    if ((i+totaldists)%100000==0) {
      cout << (i+totaldists)/100000 << " "<< mindists[i].dist << " " << arr.size()-cluster.mergecount<<" " << cluster.smatrix.size() << endl;
    }
    cluster.add(mindists[i]);
//    mindists.erase(i);
  }
  mindists.clear();
  cout << endl;

  cout << "# totaldists: " << totaldists << " totalmergedists: " << totalmergedists<< " mindists.size: " << mindists.size() << " cluster.smatrix.size: " << cluster.smatrix.size() << " otus: "<< arr.size()-cluster.mergecount << endl;

  ++step;
  if (totaldists>0){
    server.onAllReady=serverMergeResults;
    for (i=0; i<server.sockets.size(); ++i){
      if (cdists[i]<=0) continue;
      server.getClient(i).call("nodeUpdate",evararray(cluster.smerge));
    }
    for (i=0; i<cluster.smerge.size(); ++i)
      cluster.smerge[i]=-1;
    return;
  }
  cout << "# time clustering: "<< t1.lap() <<endl;
  cout << "# merge improvement: "<< (float)totalmergedists/(float)itotaldists << endl;

  cluster.save(ofile,arr);
  cout << "# done writing: "<<ofile << endl;

  exit(0);


//  for (i=0; i<mindists.size(); ++i){
//    doJoin(i);
//  }
//  cout << "# done going through distances" << endl;
}

int ncpus=100;

void doIncoming(edcserver& server)
{
  if (server.sockets.size()==ncpus)
    startComputation(server);
}

int nodePos;

ebasicarray<eseqdist> nodeGetThres(float minthres)
{
  int i;
  int count=0;
  for (i=0; nodePos-i>0; ++i){
    if (mindists[nodePos-i-1].dist<minthres) break;
//      tmparr.add(mindists[pos-i]);
  }
  
  int j;
  ebasicarray<eseqdist> tmparr;
  ldieif(nodePos-i<0 || nodePos>mindists.size(),"out of bounds: "+estr(nodePos-i)+" " + estr(nodePos));
  for (j=nodePos-i; j<nodePos; ++j){
    if (mindists[j].count)
      tmparr.add(mindists[j]);
  }
  nodePos-=i;
  return(tmparr);
//  cout << "pos: " << pos << " maxcount: " << maxcount << " thres: " << minthres<<" result: " << tmparr.size() << endl;
//  return(mindists.subset(pos-i,i));
}

ebasicarray<eseqdist> nodeGetCount(int maxcount)
{
  int i;
  int count=0;
  for (i=0; count<=maxcount && nodePos-i>0; ++i){
    if (mindists[nodePos-i-1].count>0) ++count;
//      tmparr.add(mindists[pos-i]);
  }
  
  int j;
  ebasicarray<eseqdist> tmparr;
  ldieif(nodePos-i<0 || nodePos>mindists.size(),"out of bounds: "+estr(nodePos-i)+" " + estr(nodePos));
  for (j=nodePos-i; j<nodePos; ++j){
    if (mindists[j].count)
      tmparr.add(mindists[j]);
  }
  nodePos-=i;
  return(tmparr);
//  cout << "pos: " << pos << " maxcount: " << maxcount << " thres: " << minthres<<" result: " << tmparr.size() << endl;
//  return(mindists.subset(pos-i,i));
}


int nodeComputeDistances(int node,int tnodes,float thres)
{
  int tmpi;
  nodePos=calc_dists_nogap(arr,mindists,node,tnodes,thres);
//  nodePos=calc_dists(arr,mindists,node,tnodes,thres);
  heapsort(mindists);
  return(nodePos);
}

eintarray scluster;

int nodeUpdate(eintarray& smerge)
{
  int count=0;
  int i,j;
  ebasicstrhashof<int> cmatrix;
  ebasichashmap<estr,int>::iter it;
  estr tmps; 

  // update merged sequences
  for (i=0; i<smerge.size(); ++i){
    if (smerge[i]>=0) scluster[i]=smerge[i];
  }

  for (i=nodePos-1; i>=0; --i){
    if (mindists[i].count==0) continue;
    if (smerge[mindists[i].x]>=0 || smerge[mindists[i].y]>=0){
      xy2estr(scluster[mindists[i].x],scluster[mindists[i].y],tmps);
      it=cmatrix.get(tmps);
      if (it==cmatrix.end())
        cmatrix.add(tmps,i);
      else{
        j=*it;
        mindists[i].count+=mindists[j].count;
        mindists[j].count=0;
        *it=i;
        ++count;
      }
    }
  }
  return(count);
}


int emain()
{
  estr host;
  epregister(host);
  epregister(ncpus);
  epregister(mindists);
  epregister(tbucket);
  epregister(ofile);
  eparseArgs(argvc,argv);

  epregisterClass(eseqdist);
  epregisterClassSerializeMethod(eseqdist);
  epregisterClassProperty(eseqdist,dist);
  epregisterClassProperty(eseqdist,x);
  epregisterClassProperty(eseqdist,y);

  epregisterClass(ebasicarray<eseqdist>);
  epregisterClassInheritance(ebasicarray<eseqdist>,ebasearray);
  epregisterClassMethod(ebasicarray<eseqdist>,subset);
  epregisterClassSerializeMethod(ebasicarray<eseqdist>);

  epregisterClass(earray<int>);
  epregisterClassInheritance(earray<int>,ebasearray);
  epregisterClassMethod(earray<int>,subset);
  epregisterClassSerializeMethod(earray<int>);


  epregisterFunc(nodeGetCount);
  epregisterFunc(nodeGetThres);
  epregisterFunc(nodeComputeDistances);
  epregisterFunc(nodeUpdate);

  ldieif(argvc<2,"syntax: "+efile(argv[0]).basename()+" <file>");

  int j,i2;

//  for (i=0; i<arr.size()*(arr.size()+1)/2-arr.size(); ++i)
//    dist_mat.add(0.0);

/*
  ethread thread;
  thread.wait();
  evararray vararr;

  vararr.add(new int(0));
  vararr.add(new int(arr.size()/4));
  vararr.add(dist_mat);
  vararr.add(arr);
  thread.run(efunc(calc,vararr));
*/
  int i;

  if (host.len()>0){
    load_seqs(argv[1],arr);
    scluster.reserve(arr.size());
    for (i=0; i<arr.size(); ++i)
      scluster.add(i);
    startClient(host,12345);
    return(0);
  }else{
    load_accs(argv[1],arr);
//    load_seqs(argv[1],arr);
    cluster.init(arr.size());

    edcserver server;
    registerServer();
    epregister(server);
//    server.showResult=true;
    server.onIncoming=doIncoming;
//    server.onAllReady=doAllReady;
    server.listen(12345);
//    startServer(12345);
//    epruninterpret();
    getSystem()->run();
    return(0);
  }

//  calc(0,arr.size()/4,dist_mat,arr);
//  calc(arr.size()/4,arr.size()/2,dist_mat,arr);
//  calc2(dist_mat,arr);

//  cout << "# distances: " << dist_mat.size() << endl;


  return(0);
}

