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
earray<eseqdist> mindists;
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
earray<eseqdist> tmindists[2];

int nd,od;

void doClustering2(edcserver& server);

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


  nd=0; od=1;
  server.onAllReady=doClustering;
  cout << "# getting first sorted distances" << endl;
  for (i=0; i<server.sockets.size(); ++i){
    if (cdists[i]==0) continue;
    int tmp=min(cdists[i],tbucket);
    cdists[i]-=tmp; totaldists-=tmp;
    server.getClient(i).call("mindists.subset",evararray(cdists[i],tmp));
//    server.getClient(i).eval("mindists");
//    server.getClient(i).call("doTest",evararray(100,200));
  }
}

void doClustering2(edcserver& server)
{
  int i;
  cout << "getting next" << endl;
  for (i=0; i<server.sockets.size(); ++i){
    server.getClient(i).result.clear();
    server.getClient(i).call("doTest",evararray(100,200));
//    server.getClient(i).eval("mindists");
//    server.getClient(i).call("mindists.subset",evararray(cdists[i],tmp));
//    server.getClient(i).call("iarr.subset",evararray(0,100000));
//    server.getClient(i).eval("iarr");
  }
}


void doClustering(edcserver& server)
{
  cout << "# starting clustering " << endl;
  int i;
  int count=0;
  float mindist=0.0;
  for (i=0; i<server.sockets.size(); ++i){
    if (server.getClient(i).result.isNull()){ if (mindist<cmins[i]) mindist=cmins[i]; continue; }
//    ldieif(server.getClient(i).result.isNull(),"not supposed to happen");

    earray<eseqdist> *tmparr;

    tmparr=&server.getClient(i).result.get<earray<eseqdist> >();
    cout << "# client: "<<i << " min: " << tmparr->at(0).dist << " max: " << tmparr->at(tmparr->size()-1).dist << " cdists: " << cdists[i] << " size: "<< tmparr->size() << endl;
    tmindists[nd].clear();
    mergesorted(*tmparr,tmindists[od],tmindists[nd]);
    nd=od; od=(od+1)%2;
//    mindists+=*tmparr;
    cmins[i]=tmparr->at(0).dist;
    if (mindist<cmins[i]) mindist=cmins[i];
    server.getClient(i).result.clear();
  }

  cout << "# array with "<<tmindists[od].size()<< " distances" << endl;
//  cout << "# Sorting array with "<<mindists.size()<< " distances" << endl;
//  heapsort(mindists);

  int last=tmindists[od].size()-1;
  cout << "# min: " << tmindists[od][0].dist << " minall: "<< mindist << " max: "<<tmindists[od][last].dist << endl;

/*
  cout << "# writing sorted distances to file \"sorted-dists.dat\"" << endl;
  estr datastr;
  mindists.serial(datastr);
  efile f("sorted-dists.dat");
  f.write(datastr);
  f.close();
  cout << "# done" << endl;
  exit(0);
*/


//  cout << "# checking for duplicates" << endl;
//  cluster.check(mindists);

  for (i=tmindists[od].size()-1; i>=0 && (tmindists[od][i].dist>=mindist || totaldists==0); --i){
    if (tmindists[od].size()%tbucket==0) { cout << tmindists[od].size()/tbucket << "."; flush(cout); }
    cluster.add(tmindists[od][i]);
    tmindists[od].erase(i);
  }
  cout << endl;

  cout << "# remaining distances: " << tmindists[od].size() << " cluster.smatrix.size: " << cluster.smatrix.size() << endl;

  ++step;
//  mindists.clear();
  if (totaldists>0){
    cout << "# getting next "<<tbucket<<" seqs from each node: "<<step<<" remaining dists: " << totaldists<< endl;
    for (i=0; i<server.sockets.size(); ++i){
//      if (cdists[i]==0 || tmindists[od].size() && cmins[i]<mindist) continue;
      if (cdists[i]==0 || tmindists[od].size()>server.sockets.size()*tbucket && cmins[i]<mindist) continue;
      int tmp=min(cdists[i],tbucket);
      cdists[i]-=tmp; totaldists-=tmp;
      server.getClient(i).call("mindists.subset",evararray(cdists[i],tmp));
    }
    return;
  }
  cout << "# time clustering: "<< t1.lap() <<endl;

  cluster.save(ofile,arr);
  cout << "# done writing cluster.dat" << endl;

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

int doCalc(int node,int tnodes,float thres)
{
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

  epregisterClass(earray<eseqdist>);
  epregisterClassInheritance(earray<eseqdist>,ebasearray);
  epregisterClassMethod(earray<eseqdist>,subset);
  epregisterClassSerializeMethod(earray<eseqdist>);

  epregisterClass(earray<int>);
  epregisterClassInheritance(earray<int>,ebasearray);
  epregisterClassMethod(earray<int>,subset);
  epregisterClassSerializeMethod(earray<int>);


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
    cluster.init(arr.size());

//    load_seqs(argv[1],arr);
    edcserver server;
    registerServer();
    epregister(server);
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

