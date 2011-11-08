#include <eutils/emain.h>
#include <eutils/efile.h>
#include <eutils/eheap.h>
#include <eutils/edcserver.h>
#include <eutils/esystem.h>
#include <eutils/etimer.h>
#include <eutils/etime.h>
#include <eutils/ethread.h>
#include <eutils/eoption.h>

#include "eseqclusteravg.h"

#include "cluster-common.h"

//edcclient client;
//edcserver server;

edcmpi dcmpi;


eoption<efunc> dfunc;
estrarray arr;
//ebasicarray<eseqdist> mindists;
estr ofile="cluster.dat";

float t=0.9; // clustering threshold
estr distfile;
efile fdists;

etimer t1;

int seqlen=0;

earray<estr> nodeArr;
eblockarray<eseqdist> nodeDists;
////ebasicarray<eseqdist> nodeDists;


void serverGetDistances(edcmpi& server);
void serverCluster(edcmpi& server);
void serverDistanceThreshold(edcmpi& server);
void serverMergeResults(edcmpi& server);

int step=1;
long int totaldists=0;
long int itotaldists=0;

eseqcluster      clcluster;
//eseqclusteravg      avgcluster;
eseqclustersingle slcluster;


long int transfersize=1000;
long int buffersize=10000;
long int tbucket=100000;
float tdists;

ebasicarray<long int> cdists;
ebasicarray<ebasicarray<eseqdist> > cmindists;
earray<emutex> cmindistsMutexs;
//ebasicarray<float> cmins;
eintarray cpos,cend;
eintarray cfinished;
int lastupdate;
int haveData=0;
int finishedCount=0;

float unstime=0.0;
float clstrtime=0.0;
etimer t3,t4,t5;
float t4time=0.0;

int cnode=-1;
emutex clusterMutex;


void writeDistance(efile& f,eseqdist& sdist)
{
  estr tmpstr;
  long int i;
  sdist.serial(tmpstr);
  f.write(tmpstr);
}



void serverClusterDistance(edcmpi& server);

void serverFinished(edcmpiServerNode& sclient,const estr& msg)
{
  int i;
  bool allFinished=true;
  bool foundSocket=false;
  for (i=0; i<sclient.server.nodes.size(); ++i){
    foundSocket=true;
    if (&sclient.server.getClient(i)==&sclient){
      cmindistsMutexs[i].lock();
      cfinished[i]=1;
      if (cpos[i]==cend[i])
        ++finishedCount;
      cout << "# client: " << i << " sent stream EOF. haveData: " << haveData << " finishedCount: " << finishedCount << endl;
      cmindistsMutexs[i].unlock();
    }
    if (cfinished[i]!=1) allFinished=false;
  }
  ldieif(foundSocket==false,"did not find socket?!?");
  if (!allFinished) return;
//  if (!allFinished) { serverClusterDistance(server); return; }

  if (finishedCount!=cmindists.size())
    serverClusterDistance(sclient.server);  

  lassert(finishedCount!=cmindists.size());

  cout << "# time computing distances: "<< tdists*0.001 <<endl;
  cout << "# total time clustering: "<< t1.lap()*0.001 <<endl;
  cout << "# time clustering: " << clstrtime*0.001 << endl;
  cout << "# time unserializing: " << unstime*0.001 << endl;
  cout << "# time receiving: " << t4time*0.001 << endl;
  cout << "# total distances: "<< itotaldists << endl;

  clcluster.save(ofile+".cl.otu",arr);
  cout << "# done writing complete linkage otu file: "<<ofile<<".cl.otu" << endl;
//  avgcluster.save(ofile+".avg.otu",arr);
//  cout << "# done writing average linkage otu file: "<<ofile<<".avg.otu" << endl;
  slcluster.save(ofile+".sl.otu",arr);
  cout << "# done writing single linkage otu file: "<<ofile<<".sl.otu" << endl;

  sclient.server.final();

  exit(0);
}

void serverClusterDistance(edcmpi& server)
{
  int i,j;
  float maxdist;

  for (i=0; i<cmindists.size(); ++i)
    cmindistsMutexs[i].lock();

  while (haveData==cmindists.size()-finishedCount) {
    maxdist=-1.0;
    for (i=0; i<cmindists.size(); ++i){
      if (cpos[i]==cend[i]) continue;
      if (cmindists[i][cpos[i]].dist>maxdist) { maxdist=cmindists[i][cpos[i]].dist; }
    }
    lassert(maxdist==-1.0);
    for (j=0; j<cmindists.size(); ++j){
      if (cfinished[j]==1 && cpos[j]==cend[j]) continue;
//      if (cpos[j]!=cend[j])
//        cout << "# client "<<j<<" cpos: "<< cpos[j] << " cend: " << cend[j] << " maxdist: " << maxdist<<" cfinished: " << cfinished[j] << " haveData: " << haveData << " finishedCount: " << finishedCount << endl;
      while(cpos[j]!=cend[j] && cmindists[j][cpos[j]].dist == maxdist) {
//      cout << "+ " << idist << " " << maxdist << " " << cpos[idist] << " " << cend[idist] << endl;
//        avgcluster.add(cmindists[j][cpos[j]]);
        writeDistance(fdists,cmindists[j][cpos[j]]);
//        clcluster.add(cmindists[j][cpos[j]]);
//        slcluster.add(cmindists[j][cpos[j]]);
        cpos[j]=(cpos[j]+1)%cmindists[j].size();
      }
//      cout << "# client "<<j<<" cpos: "<< cpos[j] << " cend: " << cend[j] << " maxdist: " << maxdist<<" cfinished: " << cfinished[j] << " haveData: " << haveData << " finishedCount: " << finishedCount << endl;
      if (cpos[j]==cend[j]){
        --haveData;
//        cout << "# client "<<j<<" buffer empty. cfinished: " << cfinished[j] << " haveData: " << haveData << " finishedCount: " << finishedCount << endl;
        if (cfinished[j]==1) {
          ++finishedCount;
//          cout << "# client "<<j<<" still had data but finished. haveData: " << haveData << " finishedCount: " << finishedCount << endl;
        }
      }
      if (cpos[j]==cend[j] && server.getClient(j).isChoked){
//      if ((cpos[j]==cend[j] || (cpos[j]-cend[j]+cmindists[j].size())%cmindists[j].size()>transfersize) && server.getClient(j).isChoked){
        cout << "# unchoking client: " << j << endl;
        cmindistsMutexs[j].unlock();
        server.getClient(j).unchoke();
        server.getClient(j).doProcess();
//        cout << "# done unchoking" << endl;
//        if (cpos[j]==cend[j])
//          server.getClient(j).doRecv();
        cmindistsMutexs[j].lock();
        if (cpos[j]!=cend[j] && cmindists[j][cpos[j]].dist == maxdist){ --j; continue; /*ugly hack*/ }
      }
    }
  }

  for (i=0 ; i<cmindists.size(); ++i)
    cmindistsMutexs[i].unlock();

/*
  for (i=0 ; i<cmindists.size(); ++i){
    if (server.getClient(i).isChoked && (cend[i]-cpos[i]+cmindists[i].size())%cmindists[i].size()<cmindists[i].size()/2){
      cout << "# unchoking client: " << i << endl;
      server.getClient(i).unchoke();
    }
  }
*/
}

void serverProcessDists(int i,const estr& msg,int& count)
{
  uint32_t *pstr=(uint32_t*)msg._str;
  count=*pstr;
  ++pstr;
  int j;
//  cout << "# " << i << " count: " << count << endl;
  for (j=0; j<count; ++j){
    cmindists[i][cend[i]+j].x=*pstr; ++pstr;
    cmindists[i][cend[i]+j].y=*pstr; ++pstr;
    cmindists[i][cend[i]+j].dist=*(float*)pstr; ++pstr;
//    cmindists[i][cend[i]+j].count=1;
  }
  cend[i]=(cend[i]+count)%cmindists[i].size();
}

//ebasicarray<eseqdistCount> sdists;
void serverRecvDistance(edcmpiServerNode& sclient,const estr& msg)
{
  t4.reset();
//  sdists.clear();
//  t3.reset();
//  ldieif(sdists.unserial(msg,0)==-1,"problem in received msg");
//  unstime+=t3.lap();
  bool foundServer=false;
  int i,j,count;
  for (i=0; i<sclient.server.nodes.size(); ++i){
    if (&sclient.server.getClient(i)==&sclient){
      cmindistsMutexs[i].lock();
      if (cend[i]==cpos[i])
        ++haveData;
      lassert(cend[i]==cpos[i] && cfinished[i]==1);
      if (cfinished[i]==-1){
        cfinished[i]=0;
        if (haveData==cmindists.size()){
          tdists=t1.lap();
          cout << "# time calculating distances: "<< tdists*0.001 <<endl;
        }
      }
      t3.reset();
      serverProcessDists(i,msg,count);
      unstime+=t3.lap();
      t4time+=t4.lap();
//      cout << "# " << haveData << " " << finishedCount << " " << i << " " << cpos[i] << " " << cend[i] << " " << count << endl;
      if (haveData==cmindists.size()-finishedCount && clusterMutex.trylock()){
        t5.reset();
        cmindistsMutexs[i].unlock();
        serverClusterDistance(sclient.server);
        cmindistsMutexs[i].lock();
        clstrtime+=t5.lap();
        clusterMutex.unlock();
      }
      t4.reset();
      if (cpos[i]!=cend[i] && (cpos[i]-cend[i]+cmindists[i].size())%cmindists[i].size()<=count) {
        cout << "# choking client: " << i << endl;
        sclient.choke();
      }
      cmindistsMutexs[i].unlock();
      foundServer=true;
      break;
    }
  }
  lassert(!foundServer);
  t4time+=t4.lap();
}

void savearray(efile& f,ebasicarray<eseqdist>& sdist)
{
  estr tmpstr;
  long int i;
  for (i=0; i<sdist.size(); ++i){
    tmpstr.clear();
    sdist[i].serial(tmpstr);
    f.write(tmpstr);
  }
}

void savearray(eblockarray<eseqdist>& sdist,const estr& filename)
{
  efile f(filename);
  estr tmpstr;
  long int i;
  for (i=0; i<sdist.size(); ++i){
    tmpstr.clear();
    sdist[i].serial(tmpstr);
    f.write(tmpstr);
  }
  f.close();
}

void savearray(const ebasicarray<eseqdist>& sdist,const estr& filename)
{
  estr tmpstr;
  sdist.serial(tmpstr);
  efile f(filename);
  f.write(tmpstr);
  f.close();
}


float getmaxdist()
{
  int i;
  float maxdist=-1.0;

  for (i=0; i<cmindists.size(); ++i){
    if (cpos[i]==cmindists[i].size()){ if (cdists[i]>0) return(-1.0); else continue; }
    if (cmindists[i][cpos[i]].dist>maxdist) maxdist=cmindists[i][cpos[i]].dist;
  }
  return(maxdist);
}

/*
void serverCluster(edcserver& server)
{
  long int i;
  long int count=0;
//  mindist=0.0;
//  mdist=1.0;
  for (i=0; i<server.sockets.size(); ++i){
    if (server.getClient(i).result.isNull()) continue;
//    if (server.getClient(i).result.isNull()){ if (mindist<cmins[i]) mindist=cmins[i]; if (mdist>cmins[i]) mdist=cmins[i]; continue; }

    ebasicarray<eseqdistCount> *tmparr;

    tmparr=&server.getClient(i).result.get<ebasicarray<eseqdistCount> >();
    cmindists[i]+=*tmparr;
    if (tmparr->size()>0){
      cout << "# client: "<<i << " min: " << tmparr->at(0).dist << " max: " << tmparr->at(tmparr->size()-1).dist << " cdists: " << cdists[i] << " size: "<< tmparr->size() << " ("<< cmindists[i].size()-cpos[i] << ")" << endl;
//      cmins[i]=tmparr->at(0).dist;
      cdists[i]-=tmparr->size();
      totaldists-=tmparr->size();
//      if (mindist<cmins[i]) mindist=cmins[i];
//      if (mdist>cmins[i]) mdist=cmins[i];
    }
    else
      cout << "# client: "<<i<<" empty array ("<<cmindists[i].size()-cpos[i]<<")"<<endl; 
    server.getClient(i).result.clear();
  }

//  cout << "# Sorting array with "<<mindists.size()<< " distances" << endl;
//  heapsort(mindists);

//  savearray(mindists,"distances.dat");

//  if (distfile.len())
//    savearray(fdists,mindists);

//  cout << "# min: " << mindists[0].dist << " minall: "<< mindist << " max: "<<mindists[mindists.size()-1].dist << " cluster.smatrix.size(): " << cluster.smatrix.size() << endl;
  cout << " minall: "<< mindist << " cluster.smatrix.size(): " << clcluster.smatrix.size() << endl;

  float maxdist;
  while ((maxdist=getmaxdist())>0.0) {
    for (i=0; i<cmindists.size(); ++i){
      while (cpos[i]<cmindists[i].size() && cmindists[i][cpos[i]].dist==maxdist){
        cluster.add(cmindists[i][cpos[i]]);
//        if (distfile.len())
//          writeDistance(fdists,cmindists[i][cpos[i]]);
        ++cpos[i];

        if (cluster.mergecount%10000==0 && lastupdate!=cluster.mergecount){
          lastupdate=cluster.mergecount;
          server.onAllReady=serverMergeResults;
          eintarray scluster(cluster.scluster);
          for (i=0; i<scluster.size(); ++i)
            if (cluster.smerge[i]==-1) scluster[i]=-1;
          for (i=0; i<server.sockets.size(); ++i){
            if (cdists[i]<=0) continue;
            server.getClient(i).call("nodeUpdate",evararray(scluster));
          }
          for (i=0; i<cluster.smerge.size(); ++i)
            cluster.smerge[i]=-1;
          return;
        }
//        cmindists[i].erase(0);
      }
    }
  }

  

//  cout << "# totaldists: " << totaldists << " totalmergedists: " << totalmergedists<< " mindists.size: " << mindists.size() << " cluster.smatrix.size: " << cluster.smatrix.size() << " otus: "<< arr.size()-cluster.mergecount << endl;
  cout << "# totaldists: " << totaldists << " cluster.smatrix.size: " << cluster.smatrix.size() << " otus: "<< arr.size()-cluster.mergecount << endl;

  ++step;
  if (totaldists>0){
    cout << "# getting next "<<tbucket<<" seqs from each node: "<<step<<" remaining dists: " << totaldists<< endl;
    for (i=0; i<server.sockets.size(); ++i){
      if (cdists[i]<=0 || cpos[i]<cmindists[i].size()) continue;
      cpos[i]=0;
      cmindists[i].clear(); 
      server.getClient(i).call("nodeGetCount",evararray(tbucket));
    }
    return;
  }
  cout << "# time computing distances: "<< tdists*0.001 <<endl;
  cout << "# time clustering: "<< t1.lap()*0.001 <<endl;
  cout << "# total distances: "<< itotaldists << endl;

  cluster.save(ofile,arr);
  cout << "# done writing: "<<ofile << endl;

  exit(0);


//  for (i=0; i<mindists.size(); ++i){
//    doJoin(i);
//  }
//  cout << "# done going through distances" << endl;
}
*/

int ncpus=32;

long int nodePos;

/*
ebasicarray<eseqdist> nodeGetThres(float minthres,long int maxcount)
{
  cerr << "nodePos: " << nodePos<< endl;
  long int i;
  long int count=0;
  for (i=0; nodePos-i>0; ++i){
    if (nodeDists[nodePos-i-1].dist<minthres || nodeDists[nodePos-i-1].dist==minthres && i>=maxcount) break;
//      tmparr.add(mindists[pos-i]);
  }
  
  long int j;
  ebasicarray<eseqdist> tmparr;
  ldieif(nodePos-i<0 || nodePos>nodeDists.size(),"out of bounds: "+estr(nodePos-i)+" " + estr(nodePos));
  for (j=nodePos-i; j<nodePos; ++j){
//    if (mindists[j].count)
      tmparr.add(nodeDists[j]);
  }
  nodePos-=i;
  cerr << "nodePos: " << nodePos <<" nodeGetThres: " << tmparr.size() << endl;
  return(tmparr);
//  cout << "pos: " << pos << " maxcount: " << maxcount << " thres: " << minthres<<" result: " << tmparr.size() << endl;
//  return(mindists.subset(pos-i,i));
}

ebasicarray<eseqdistCount> nodeGetCount(long int maxcount)
{
  cerr << "nodePos: " << nodePos<< endl;
  long int j;
  ebasicarray<eseqdistCount> tmparr;
  for (;tmparr.size()<maxcount && nodePos>=0l;--nodePos){
    if (cluster.dists[nodePos].count>0)
      tmparr.add(cluster.dists[nodePos]);
  }
  cerr << "nodePos: " << nodePos <<" nodeGetCount: " << tmparr.size() << endl;
  return(tmparr);
//  return(mindists.subset(nodePos,maxcount));
}
*/
/*
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
*/

int nthreads=4;
etaskman taskman;
emutex mutexDists;
int partsFinished=0;
int partsTotal=10000;

void nodeMakeDists(int count,estr& msg)
{
  msg.reserve(sizeof(uint32_t)*3*count+sizeof(uint32_t));
  uint32_t *pstr=(uint32_t*)msg._str;
  ++pstr;
  int i;
  for (i=0; i<count && nodePos>=0l; ++i,--nodePos){
    *pstr=nodeDists[nodePos].x; ++pstr;
    *pstr=nodeDists[nodePos].y; ++pstr;
    *(float*)pstr=nodeDists[nodePos].dist; ++pstr;
  }
  *(uint32_t*)msg._str=i;
  msg._strlen=sizeof(uint32_t)*3*i+sizeof(uint32_t);
//  cerr << "# sent " << i << " dists" << endl;
}

void nodeSendDistances(edcmpi& client)
{
  while (nodePos>=0l){
    cerr << cnode << " sending starting from nodePos: " << nodePos<< endl;
    if (nodePos==-1l) return;

    int j;
    estr tmpdata;
    do {
      tmpdata.clear();
      nodeMakeDists(transfersize,tmpdata);
    } while (client.nodeSendMsg(2,tmpdata) && nodePos>=0l);

    cerr << cnode << " sending ended at nodePos: " << nodePos<< endl;
  }
  cerr << "Sending EOF: " << client.nodeSendMsg(3,"") << endl;
}

void nodeComputeDistances(edcmpi& client,int node,int tnodes,float thres)
{
  cnode=node;
  cerr << cnode << " computing distances" << endl;
  if ((long int)partsTotal>=(nodeArr.size()-1l)*nodeArr.size()/(20l*tnodes)) partsTotal=(nodeArr.size()-1l)*nodeArr.size()/(20l*tnodes);
  int i;
  for (i=0; i<partsTotal; ++i){
    taskman.addTask(dfunc.value(),evararray(mutexDists,nodeArr,nodeDists,(const int&)seqlen,(const int&)(node*partsTotal+i),(const int&)(partsTotal*tnodes),(const float&)thres));
//    taskman.addTask(dfunc.value(),evararray(nodeArr,nodeDists,(const int&)seqlen,(const int&)(node*partsTotal+i),(const int&)(partsTotal*tnodes),(const float&)thres));
  }
  taskman.wait();

  cerr << cnode << " finished computing distances" << endl;

  mutexDists.lock();
  nodeDists.sort();
  nodePos=nodeDists.size()-1l;
  cerr << cnode << " partsFinished: " << partsFinished << " partsTotal: " << partsTotal<< endl;
  cerr << cnode << " nodePos: " << nodePos<< endl;
  mutexDists.unlock();

  nodeSendDistances(client);

  client.final();
  exit(0);
//  return(nodePos+1l);
}

/*
long int nodeUpdate(eintarray& scluster)
{
  // update merged sequences
  int i;
  if (clcluster.smerge.size()==0){
    clcluster.scluster.reserve(scluster.size());
    clcluster.smerge.reserve(scluster.size());
    for (i=0; i<scluster.size(); ++i){
      clcluster.smerge.add(-1);
      clcluster.scluster.add(i);
    }
  }
  for (i=0; i<scluster.size(); ++i){
    if (scluster[i]>=0) { clcluster.scluster[i]=scluster[i]; clcluster.smerge[i]=scluster[i]; }
  }
  return(clcluster.update(nodePos-1));
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


/*
int emain()
{
  cout << "# " << date() << endl;
  cout << "# " << args2str(argvc,argv) << endl;

  dfunc.choice=0;
//  dfunc.add("gap",gap_calc_dists);
//  dfunc.add("gap+noise",gapnoise_calc_dists);
  dfunc.add("nogap",t_calc_dists<earray<estr>,eseqdist,eblockarray<eseqdist>,dist_nogap_compressed>);
  dfunc.add("gap",t_calc_dists<earray<estr>,eseqdist,eblockarray<eseqdist>,dist_compressed>);
  dfunc.add("tamura",t_calc_dists<earray<estr>,eseqdist,eblockarray<eseqdist>,dist_tamura_compressed>);
  dfunc.add("gap+noise",t_calc_dists_noise<earray<estr>,eseqdist,eblockarray<eseqdist>,dist_compressed>);
  dfunc.add("nogap+noise",t_calc_dists_noise<earray<estr>,eseqdist,eblockarray<eseqdist>,dist_nogap_compressed>);
  dfunc.add("tamura+noise",t_calc_dists_noise<earray<estr>,eseqdist,eblockarray<eseqdist>,dist_tamura_compressed>);

  epregisterClass(eoption<efunc>);
  epregisterClassMethod2(eoption<efunc>,operator=,int,(const estr& val));

  epregister(dfunc);

  estr host;

  epregister(t);
  epregister(host);
  epregister(ncpus);
  epregister(nthreads);
//  epregister(mindists);
  epregister(tbucket);
  epregister(ofile);
  epregister(distfile);
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

  epregisterClass(eseqdistCount);
  epregisterClassSerializeMethod(eseqdistCount);
  epregisterClassProperty(eseqdistCount,dist);
  epregisterClassProperty(eseqdistCount,x);
  epregisterClassProperty(eseqdistCount,y);
  epregisterClassProperty(eseqdistCount,count);

  epregisterClass(ebasicarray<eseqdistCount>);
  epregisterClassInheritance(ebasicarray<eseqdistCount>,ebasearray);
  epregisterClassMethod(ebasicarray<eseqdistCount>,subset);
  epregisterClassSerializeMethod(ebasicarray<eseqdistCount>);

  epregisterClass(earray<int>);
  epregisterClassInheritance(earray<int>,ebasearray);
  epregisterClassMethod(earray<int>,subset);
  epregisterClassSerializeMethod(earray<int>);


  epregisterFunc(nodeComputeDistances);
//  epregisterFunc(nodeUpdate);

  ldieif(argvc<2,"syntax: "+efile(argv[0]).basename()+" <file>");

 


  int i,i2,j;

  if (host.len()>0){
    distfile="";

    load_seqs_compressed(argv[1],nodeArr,seqlen);

    cout << "# creating " << nthreads << " threads" << endl;

    taskman.createThread(nthreads);

    int pipefd[2];
    pipe(pipefd);
    dup2(pipefd[1],1);

    client.outpipe=pipefd[0];
    getSystem()->addReadCallback(client.outpipe,efunc(client,&edcclient::sendOutput),evararray());

    client.connect(host,12345);
    cerr << " waiting for command" << endl;
  }else{
    ldieif(!distfile.len(),"no distance file specified");
    fdists.open(distfile,"w");

    load_accs(argv[1],arr);
//    load_seqs(argv[1],arr);
//    avgcluster.init(arr.size(),ofile+".avg.dat");
    clcluster.init(arr.size(),ofile+".cl.dat");
    slcluster.init(arr.size(),ofile+".sl.dat");

    registerServer();
    epregister(server);
    server.callbacks.add(serverRecvDistance);
    server.callbacks.add(serverFinished);
//    server.showResult=true;
    server.onIncoming=doIncoming;
//    server.onAllReady=doAllReady;
    server.listen(12345);
  }

  getSystem()->run();

  return(0);
}
*/


void doStartClient(edcmpi& node)
{
  distfile="";
  load_seqs_compressed(argv[1],nodeArr,seqlen);

  cout << "# creating " << nthreads << " threads" << endl;

  taskman.createThread(nthreads);

  int pipefd[2];
  pipe(pipefd);
  dup2(pipefd[1],1);

  node.outpipe=pipefd[0];
//  getSystem()->addReadCallback(node.outpipe,efunc(node,&edcmpiNode::sendOutput),evararray());

  nodeComputeDistances(node,node.rank-1,node.numprocs-1,0.9);
}

void doStartServer(edcmpi& server)
{
  ldieif(!distfile.len(),"no distance file specified");
  fdists.open(distfile,"w");

  load_accs(argv[1],arr);
//  avgcluster.init(arr.size(),ofile+".avg.dat");
  clcluster.init(arr.size(),ofile+".cl.dat",argv[1]);
  slcluster.init(arr.size(),ofile+".sl.dat",argv[1]);

  server.callbacks.add(serverRecvDistance);
  server.callbacks.add(serverFinished);

  t1.reset();
  cout << "# starting distributed computation" << endl;

//  sdists.reserve(10000);
  int i;
  for (i=0; i<server.nodes.size(); ++i){
    cmindists.add(ebasicarray<eseqdist>());
    cmindistsMutexs.add(emutex());
    cmindists[i].init(buffersize);
    cpos.add(0);
    cend.add(0);
    cfinished.add(-1);
  }
}

int emain()
{
  cout << "# " << date() << endl;
  cout << "# " << args2str(argvc,argv) << endl;

  dfunc.choice=0;
//  dfunc.add("gap",gap_calc_dists);
//  dfunc.add("gap+noise",gapnoise_calc_dists);

  dfunc.add("gap",t_calc_dists<earray<estr>,eseqdist,eblockarray<eseqdist>,dist_compressed>);
  dfunc.add("nogap",t_calc_dists<earray<estr>,eseqdist,eblockarray<eseqdist>,dist_nogap_compressed>);
  dfunc.add("tamura",t_calc_dists<earray<estr>,eseqdist,eblockarray<eseqdist>,dist_tamura_compressed>);
  dfunc.add("gap+noise",t_calc_dists_noise<earray<estr>,eseqdist,eblockarray<eseqdist>,dist_compressed>);
  dfunc.add("nogap+noise",t_calc_dists_noise<earray<estr>,eseqdist,eblockarray<eseqdist>,dist_nogap_compressed>);
  dfunc.add("tamura+noise",t_calc_dists_noise<earray<estr>,eseqdist,eblockarray<eseqdist>,dist_tamura_compressed>);

  epregisterClass(eoption<efunc>);
  epregisterClassMethod2(eoption<efunc>,operator=,int,(const estr& val));

  epregister(dfunc);

  estr host;

  epregister(t);
  epregister(host);
  epregister(ncpus);
  epregister(nthreads);
//  epregister(mindists);
  epregister(tbucket);
  epregister(ofile);
  epregister(distfile);
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

  epregisterClass(eseqdistCount);
  epregisterClassSerializeMethod(eseqdistCount);
  epregisterClassProperty(eseqdistCount,dist);
  epregisterClassProperty(eseqdistCount,x);
  epregisterClassProperty(eseqdistCount,y);
  epregisterClassProperty(eseqdistCount,count);

  epregisterClass(ebasicarray<eseqdistCount>);
  epregisterClassInheritance(ebasicarray<eseqdistCount>,ebasearray);
  epregisterClassMethod(ebasicarray<eseqdistCount>,subset);
  epregisterClassSerializeMethod(ebasicarray<eseqdistCount>);

  epregisterClass(earray<int>);
  epregisterClassInheritance(earray<int>,ebasearray);
  epregisterClassMethod(earray<int>,subset);
  epregisterClassSerializeMethod(earray<int>);


  epregisterFunc(nodeComputeDistances);
//  epregisterFunc(nodeUpdate);

  ldieif(argvc<2,"syntax: "+efile(argv[0]).basename()+" <file>");

  cout << "# starting mpi cluster" << endl;
  dcmpi.onStartClient=doStartClient;
  dcmpi.onStartServer=doStartServer;

  cout << "# initializing MPI library" << endl;
  dcmpi.init(argvc,&argv);
  dcmpi.final();

/*
//  eparseArgs(argvc,argv);

  int res;
  res=MPI_Init(&argvc,&argv);
  if (res!=MPI_SUCCESS){
    lerror("Starting MPI program");
    MPI_Abort(MPI_COMM_WORLD,res);
  }
  MPI_Comm_size(MPI_COMM_WORLD, &dcomp.numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &dcomp.rank);
  MPI_Get_processor_name(dcomp.processor_name, &dcomp.namelen);
  cout << "# number of processes: " << dcomp.numprocs << " rank: "<< dcomp.rank << endl;

  if (dcomp.rank==0)
    doStartServer();
  else 
    doStartClient(); 
 
  MPI_Finalize();
*/

  return(0);
}

