#include <eutils/emain.h>
#include <eutils/efile.h>
#include <eutils/eheap.h>
#include <eutils/edcserver.h>

inline float dist(const estr& s1,const estr& s2)
{
  int i;
  int count=0;
  for (i=0; i<s1.len(); ++i){
    if (s1[i]==s2[i]) ++count;
  }
  return((float)count/(float)s1.len());
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

estrhash arr;
ebasicarray<float> dist_mat;

ebasicarray<float>& calc(int start,int end)
{
  int c=0;
  int i,i2,j;

  dist_mat.clear();
  dist_mat.reserve((start-end)*(arr.size()-1));
//  for (i=0; i<(start-end)*(arr.size()-1); ++i)
//    dist_mat.add(0.0);

  for (i=start; i<end; ++i){
//    getL(c,arr.size(),i,i+1);
    for (j=i+1; j<arr.size(); ++j)
      dist_mat.add(dist(arr.values(i),arr.values(j)));
    i2=arr.size()-i-1;
    for (j=i2+1; j<arr.size(); ++j)
      dist_mat.add(dist(arr.values(i2),arr.values(j)));
  }
  if (end==arr.size()/2 && arr.size()%2==1){
//    getL(c,arr.size(),i,i+1);
    i=arr.size()/2;
    for (j=i+1; j<arr.size(); ++j)
      dist_mat.add(dist(arr.values(i),arr.values(j)));
  }
  return(dist_mat);
}

ebasicarray<float> mindist;
eintarray mindist_id,mindist_id2;

void doJoinCluster(edcserver& server)
{
  // join the clusters with the minimal distance

  // get all distances from dist nodes regarding the two clusters, and update the min distances in the whole heap

  // reheapify

}

int level; //=log2(ncpus+1)-1

void doneDistributedHeapify(edcserver& server)
{
  int i,root,child;
  // heapify levels in master node

  for (i=mindist.size()/2; i>=0; --i){
    root=i;
    while (root<<1 < mindist.size()){
      child=root<<1;
      if (child+1<tmpsize && mindist[child] < mindist[child+1])
        ++child;
      if (mindist[root] < mindist[child]){
        mindist.swap(root,child);
        root=child;
//        if (2*(ncpus-1)
      } else break;
  }

  // whenever an item sifts-down to a distributed node, call
  {
    server.getClient(i).call("doLocalHeapify",evararray(item));
    return; // wait for distnode heapify to finish and then continue master heapify
  }
  
  //After this is done we should have the highest identity in the root node of our distributed heap
  server.onAllReady=doJoinCluster;
}

void doDistributedHeapify(edcserver& server)
{
  int i;
  for (i=0; i<server.sockets.size(); ++i)
    server.getClient(i).call("doLocalHeapify",evararray());

  server.onAllReady=doneDistributedHeapify;
}




int calc_mindist(int start,int end,float thres)
{
  int c=0;
  int i,i2,j;

//  dist_mat.clear();
//  for (i=0; i<(start-end)*(arr.size()-1); ++i)
//    dist_mat.add(0.0);

  float tmpid;
  int count=0;


  for (i=start; i<end; ++i){
//    getL(c,arr.size(),i,i+1);
    for (j=i+1; j<arr.size(); ++j){
      tmpid=dist(arr.values(i),arr.values(j));
      if (tmpid>thres) { ++count; mindist.add(tmpid); mindist_id.add(i); mindist_id2.add(j); }
//      dist_mat.add(dist(arr.values(i),arr.values(j)));
    }
    i2=arr.size()-i-1;
    for (j=i2+1; j<arr.size(); ++j){
      tmpid=dist(arr.values(i2),arr.values(j));
      if (tmpid>thres) { ++count; mindist.add(tmpid); mindist_id.add(i2); mindist_id2.add(j); }
//      dist_mat.add(dist(arr.values(i2),arr.values(j)));
    }
  }
  if (end==arr.size()/2 && arr.size()%2==1){
//    getL(c,arr.size(),i,i+1);
    i=arr.size()/2;
    for (j=i+1; j<arr.size(); ++j){
      tmpid=dist(arr.values(i),arr.values(j));
      if (tmpid>thres) { ++count; mindist.add(tmpid); mindist_id.add(i); mindist_id2.add(j); }
//      dist_mat.add(dist(arr.values(i),arr.values(j)));
    }
  }

  return(count);
//  return(dist_mat);
}

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

void startComputation(edcserver& server)
{
//  calc(0,arr.size()/4,dist_mat,arr);
  int i;

  cout << "# starting distributed computation" << endl;

  for (i=0; i<server.sockets.size(); ++i)
    server.getClient(i).call("calc_mindist",evararray(i*arr.size()/(2*server.sockets.size()),(i+1)*arr.size()/(2*server.sockets.size()),0.9));
}

int state=0;

void getDistances(edcserver& server)
{
  int i;
  int count=0;
  for (i=0; i<server.sockets.size(); ++i){
    if (server.getClient(i).result.isNull()) continue;
    count+=server.getClient(i).result.get<int>();
  }
  cout << "# total distances under threshold: " << count << endl;
  for (i=0; i<server.sockets.size(); ++i){
    server.getClient(i).call("mindist.subset",evararray(0,5));
  }
}

void doClustering(edcserver& server)
{
  cout << "# starting clustering " << endl;
//  int i;
//  int count=0;
//  for (i=0; i<server.sockets.size(); ++i){
//    if (server.getClient(i).result.isNull()) continue;
//    cout << server.getClient(i).result.get<ebasicarray<float> >();
//  }
}

void doAllReady(edcserver& server)
{
  cout << "do AllReady!" << endl;
  switch (state){
    case 0: { getDistances(server); state=1; } break;
    case 1: doClustering(server);
  }
}


int ncpus=10;

void doIncoming(edcserver& server)
{
  if (server.sockets.size()==ncpus)
    startComputation(server);
}

int emain()
{
  estr host;
  epregister(host);
  epregister(ncpus);
  epregister(mindist);
  epregister(mindist_id);
  epregister(mindist_id2);
  eparseArgs(argvc,argv);

  epregisterClass(ebasicarray<float>);
  epregisterClassInheritance(ebasicarray<float>,ebasearray);
  epregisterClassSerializeMethod(ebasicarray<float>);
  epregisterFunc(calc);
  epregisterFunc(calc_mindist);
  epregisterFunc(get_mindist);

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
  load_seqs(argv[1],arr);
  if (host.len()>0){
    startClient(host,12345);
    return(0);
  }else{
    edcserver server;
    registerServer();
    epregister(server);
    server.onIncoming=doIncoming;
    server.onAllReady=doAllReady;
    server.listen(12345);
//    startServer(12345);
    epruninterpret();
    return(0);
  }

//  calc(0,arr.size()/4,dist_mat,arr);
//  calc(arr.size()/4,arr.size()/2,dist_mat,arr);
//  calc2(dist_mat,arr);

//  cout << "# distances: " << dist_mat.size() << endl;


  return(0);
}

