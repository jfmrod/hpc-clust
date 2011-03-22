#include <eutils/emain.h>
#include <eutils/efile.h>
#include <eutils/eheap.h>
#include <eutils/edcserver.h>
#include <eutils/esystem.h>
#include <eutils/etimer.h>

#include "cluster-common.h"

//estrhash arr;
estrarray arr;
ebasicarray<float> dist_mat;
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
void doClustering(edcserver& server);

void startComputation(edcserver& server)
{
  t1.reset();
  cout << "# starting distributed computation" << endl;

  int i;
  server.onAllReady=getDistances;
  for (i=0; i<server.sockets.size(); ++i)
    server.getClient(i).call("doCalc",evararray(i,server.sockets.size(),0.9));
}

int step=1;
long int totaldists=0;
eintarray cdists;
eseqcluster cluster;
int tbucket=100000;

ebasicarray<float> cmins;

void getDistances(edcserver& server)
{
  cout << "# getting distances" << endl;
  int i;
  for (i=0; i<server.sockets.size(); ++i){
//    ldieif(server.getClient(i).result.isNull(),"not supposed to happen");
    if (server.getClient(i).result.isNull()) continue;
    cdists.add(server.getClient(i).result.get<int>());
    cmins.add(1.0);
    totaldists+=server.getClient(i).result.get<int>();
  }
  cout << "# total distances under threshold: " << totaldists << endl;
  cout << "# time calculating distance: "<< t1.lap() <<endl;


  server.onAllReady=doClustering;
  cout << "# getting first sorted distances" << endl;
  for (i=0; i<server.sockets.size(); ++i){
    if (cdists[i]==0) continue;
    server.getClient(i).call("doGet",evararray(cdists[i],tbucket,0.0));
//    server.getClient(i).call("mindists.subset",evararray(cdists[i]-1-tbucket,tbucket));

/*
    int tmp=min(cdists[i],tbucket);
    cdists[i]-=tmp; totaldists-=tmp;
    cdists[i]-=tmp; totaldists-=tmp;
    server.getClient(i).call("mindists.subset",evararray(cdists[i],tmp));
*/
  }
}


void doClustering(edcserver& server)
{
  cout << "# starting clustering " << endl;
  int i;
  int count=0;
  float mindist=0.0;
  float mdist=1.0;
  for (i=0; i<server.sockets.size(); ++i){
    if (server.getClient(i).result.isNull()){ if (mindist<cmins[i]) mindist=cmins[i]; if (mdist>cmins[i]) mdist=cmins[i]; continue; }
//    ldieif(server.getClient(i).result.isNull(),"not supposed to happen");

    ebasicarray<eseqdist> *tmparr;

    tmparr=&server.getClient(i).result.get<ebasicarray<eseqdist> >();
//    ldieif(tmparr->size()==0,"size is zero!!");
    mindists+=*tmparr;
    if (tmparr->size()>0){
      cout << "# client: "<<i << " min: " << tmparr->at(0).dist << " max: " << tmparr->at(tmparr->size()-1).dist << " cdists: " << cdists[i] << " size: "<< tmparr->size() << endl;
      cmins[i]=tmparr->at(0).dist;
      cdists[i]-=tmparr->size();
      totaldists-=tmparr->size();
      if (mindist<cmins[i]) mindist=cmins[i];
      if (mdist>cmins[i]) mdist=cmins[i];
    }else
      cout << "# client: "<<i<<" empty array"<<endl; 
    server.getClient(i).result.clear();
  }

  cout << "# Sorting array with "<<mindists.size()<< " distances" << endl;
  heapsort(mindists);

  cout << "# min: " << mindists[0].dist << " minall: "<< mindist << " max: "<<mindists[mindists.size()-1].dist << endl;

  cluster.taddtime=0.0; cluster.tmergetime=0.0; cluster.taddcount=0; cluster.taddtime=0;
  for (i=mindists.size()-1; i>=0 && (mindists[i].dist>=mindist || totaldists==0); --i){
    if ((i+totaldists)%tbucket==0) {
      cout << (i+totaldists)/tbucket << " "<< mindists[i].dist << " " << arr.size()-cluster.mergecount<<" " << cluster.smatrix.size() << " avgmergetime: "<< cluster.tmergetime/cluster.tmergecount <<" avgaddtime: "<< cluster.taddtime/cluster.taddcount <<  endl;
      cluster.taddtime=0.0; cluster.tmergetime=0.0; cluster.taddcount=0; cluster.taddtime=0;
    }
    cluster.add(mindists[i]);
    mindists.erase(i);
  }
  cout << endl;

  cout << "# remaining distances: " << mindists.size() << " cluster.smatrix.size: " << cluster.smatrix.size() << " otus: "<< arr.size()-cluster.mergecount << endl;

  ++step;
//  mindists.clear();
  if (totaldists>0){
    bool nothingtodo=true;
    if (mindists.size()==0) mdist=0.0;
    cout << "# getting next "<<tbucket<<" seqs from each node: "<<step<<" remaining dists: " << totaldists<< endl;
    for (i=0; i<server.sockets.size(); ++i){
      if (cdists[i]==0) continue;
//      if (cdists[i]==0 || mindists.size() && cmins[i]<mindist) continue;
//      if (cdists[i]==0 || mindists.size()>server.sockets.size()*tbucket && cmins[i]<mindist) continue;
      server.getClient(i).call("doGet",evararray(cdists[i],tbucket,mdist));
      nothingtodo=false;
/*
      int tmp=min(cdists[i],tbucket);
      cdists[i]-=tmp; totaldists-=tmp;
      server.getClient(i).call("mindists.subset",evararray(cdists[i],tmp));
*/
    }
    if (nothingtodo)
      ldie("nothing to do!!!");
    return;
  }
  cout << "# time clustering: "<< t1.lap() <<endl;

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

ebasicarray<eseqdist> doGet(int pos,int maxcount,float minthres)
{
  int i;
  earray<eseqdist> tmparr;
  for (i=0; i<=maxcount && pos-i>0; ++i){
    if (mindists[pos-i-1].dist<minthres)
      break;
//      tmparr.add(mindists[pos-i]);
  }
  if (i==0) return(ebasicarray<eseqdist>());
//  cout << "pos: " << pos << " maxcount: " << maxcount << " thres: " << minthres<<" result: " << tmparr.size() << endl;
  return(mindists.subset(pos-i,i));
}


int doCalc(int node,int tnodes,float thres)
{
//  return(calc_dists(arr,mindists,node,tnodes,thres));
  return(calc_dists_nogap(arr,mindists,node,tnodes,thres));
}

earray<int> iarr;

int emain()
{
  estr host;
  epregister(host);
  epregister(ncpus);
  epregister(mindists);
  epregister(iarr);
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


  epregisterFunc(doGet);
  epregisterFunc(doCalc);

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
  if (host.len()>0){
    load_seqs(argv[1],arr);
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

