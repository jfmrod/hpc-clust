#include <eutils/emain.h>
#include <eutils/efile.h>
#include <eutils/eheap.h>
#include <eutils/edcserver.h>
#include <eutils/esystem.h>
#include <eutils/etimer.h>
#include <eutils/etime.h>
#include <eutils/ethread.h>
#include <eutils/eoption.h>

#include "hpc-clust-config.h"
#include "eseqclusteravg.h"

#include "cluster-common.h"

edcmpi dcmpi;

ebasicarray<long> uniqind;

typedef float (*t_dfunc)(const estr&,const estr&,int);

class edistfunc
{
 public:
  efunc calcfunc;
  t_dfunc calcfunc_single;
  edistfunc(const efunc& _calcfunc,t_dfunc _calcfunc_single): calcfunc(_calcfunc),calcfunc_single(_calcfunc_single) {}
};

eoption<edistfunc> dfunc;
estrarray arr;
estr ofile;

float t=0.9; // clustering threshold
efile fdists;

etimer t1;

int seqlen=0;

earray<estr> nodeArr;
eblockarray<eseqdist> nodeDists;

void serverGetDistances(edcBaseServer& server);
void serverCluster(edcBaseServer& server);
void serverDistanceThreshold(edcBaseServer& server);
void serverMergeResults(edcBaseServer& server);

int step=1;
long int totaldists=0;
long int itotaldists=0;

bool dists=false;
bool sl=false;
bool cl=false;
bool al=false;

eseqcluster        clcluster;
eseqclusteravg     alcluster;
eseqclustersingle  slcluster;

int nthreads=1;

long int transfersize=1000;
long int buffersize=10000;
long int tbucket=100000;
float tdists;

ebasicarray<long> cdists;
ebasicarray<ebasicarray<eseqdist> > cmindists;
earray<emutex> cmindistsMutexs;
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

float radixKey(eblockarray<eseqdist>& dists,long int i)
{
  return(dists[i].dist);
}

template <class T>
void seq(T& arr,long n)
{
  arr.clear();
  arr.reserve(n);
  int i;
  for (i=0; i<n; ++i)
    arr.add(i);
}

template <class T>
void randperm(T& arr)
{
  long i,j;
  for (i=arr.size()-1; i>0; --i){
    j=(long)(rnd.uniform()*(i+1));
    if (j!=i) arr.swap(i,j);
  }
}


void writeDistance(efile& f,eseqdist& sdist)
{
  estr tmpstr;
  long int i;
  sdist.serial(tmpstr);
  f.write(tmpstr);
}



void serverClusterDistance(edcBaseServer& server);

void serverFinished(edcBaseServerNode& sclient,const estr& msg)
{
  long i;
  bool allFinished=true;
  bool foundSocket=false;
  for (i=0; i<sclient.server.nodeCount(); ++i){
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

  if (finishedCount!=cmindists.size())
    serverClusterDistance(sclient.server);  

  lassert(finishedCount!=cmindists.size());

  cout << "# time computing distances: "<< tdists*0.001 <<endl;
  cout << "# total time clustering: "<< t1.lap()*0.001 <<endl;
  cout << "# time clustering: " << clstrtime*0.001 << endl;
  cout << "# time unserializing: " << unstime*0.001 << endl;
  cout << "# time receiving: " << t4time*0.001 << endl;
  cout << "# total distances: "<< itotaldists << endl;

  sclient.server.final();
  exit(0);
}

void serverClusterDistance(edcBaseServer& server)
{
  long i,j;
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
      while(cpos[j]!=cend[j] && cmindists[j][cpos[j]].dist == maxdist) {
        if (dists)
          writeDistance(fdists,cmindists[j][cpos[j]]);
        if (al)
          alcluster.add(cmindists[j][cpos[j]]);
        if (cl)
          clcluster.add(cmindists[j][cpos[j]]);
        if (sl)
          slcluster.add(cmindists[j][cpos[j]]);
        cpos[j]=(cpos[j]+1)%cmindists[j].size();
      }
      if (cpos[j]==cend[j]){
        --haveData;
        if (cfinished[j]==1) {
          ++finishedCount;
        }
      }
      if (cpos[j]==cend[j] && server.getClient(j).isChoked){
        cout << "# unchoking client: " << j << endl;
        cmindistsMutexs[j].unlock();
        server.getClient(j).unchoke();
        server.getClient(j).doProcess();
        cmindistsMutexs[j].lock();
        if (cpos[j]!=cend[j] && cmindists[j][cpos[j]].dist == maxdist){ --j; continue; /*ugly hack*/ }
      }
    }
  }

  for (i=0 ; i<cmindists.size(); ++i)
    cmindistsMutexs[i].unlock();
}

void serverProcessDists(long i,const estr& msg,long& count)
{
  if (msg.len()==0) { count=0; return; }
  uint32_t *pstr=(uint32_t*)msg._str;
  count=*pstr;
  ++pstr;
  long j;
  for (j=0; j<count; ++j){
    cmindists[i][cend[i]+j].x=*reinterpret_cast<INDTYPE*>(pstr); pstr+=sizeof(INDTYPE)/sizeof(uint32_t);
    cmindists[i][cend[i]+j].y=*reinterpret_cast<INDTYPE*>(pstr); pstr+=sizeof(INDTYPE)/sizeof(uint32_t);
    cmindists[i][cend[i]+j].dist=*reinterpret_cast<float*>(pstr); ++pstr;
  }
  cend[i]=(cend[i]+count)%cmindists[i].size();
}

void serverRecvDistance(edcBaseServerNode& sclient,const estr& msg)
{
  t4.reset();
  bool foundServer=false;
  long i,j,count;
  for (i=0; i<sclient.server.nodeCount(); ++i){
    if (&sclient.server.getClient(i)==&sclient){
      cmindistsMutexs[i].lock();
      if (cend[i]==cpos[i] && msg.len()>0)
        ++haveData;
      lassert(cend[i]==cpos[i] && cfinished[i]==1);
      if (cfinished[i]==-1){
        cfinished[i]=0;
        if (haveData==cmindists.size()-finishedCount){
          tdists=t1.lap();
          cout << "# time calculating distances: "<< tdists*0.001 <<endl;
        }
      }
      t3.reset();
      serverProcessDists(i,msg,count);
      unstime+=t3.lap();
      t4time+=t4.lap();
      if (msg.len()==0 && cfinished[i]!=1){
        cout << "# finished client: "<< i <<endl;
        cfinished[i]=1;
        ++finishedCount;
      }
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

void serverStartComputation(edcBaseServer& server)
{
  t1.reset();
  cout << "# starting distributed computation" << endl;

  long i,j;
  for (i=0; i<server.nodeCount(); ++i){
    cmindists.add(ebasicarray<eseqdist>());
    cmindistsMutexs.add(emutex());
    cmindists[i].init(buffersize);
    cpos.add(0);
    cend.add(0);
    cfinished.add(-1);
  }

  for (i=0; i<server.nodeCount(); ++i)
    server.getClient(i).call("nodeComputeDistances",evararray(uniqind,(const long int&)i,(const long int&)server.nodeCount(),(const float&)t,(const int&)nthreads));
  cout << "# master) finished calling computation function on nodes" << endl;
}

void savearray(efile& f,ebasicarray<eseqdist>& sdist)
{
  estr tmpstr;
  long i;
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
  long i;
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
  long i;
  float maxdist=-1.0;

  for (i=0; i<cmindists.size(); ++i){
    if (cpos[i]==cmindists[i].size()){ if (cdists[i]>0) return(-1.0); else continue; }
    if (cmindists[i][cpos[i]].dist>maxdist) maxdist=cmindists[i][cpos[i]].dist;
  }
  return(maxdist);
}

int ncpus=32;

void doIncoming(esocket& socket)
{
  edcserver& server(dynamic_cast<edcserver&>(socket));
  cout << "server.sockets.size(): " << server.sockets.size() << endl;
  if (server.sockets.size()==ncpus)
    serverStartComputation(server);
}

long nodePos;


etaskman taskman;
emutex mutexDists;
long partsFinished=0l;
long partsTotal=10000l;
//long int partsTotal=1l;

void nodeMakeDists(int count,estr& msg)
{
  msg.reserve((sizeof(float)+2*sizeof(INDTYPE))*count+sizeof(uint32_t));
  uint32_t *pstr=(uint32_t*)msg._str;
  ++pstr;
  int i;
  for (i=0; i<count && nodePos>=0l; ++i,--nodePos){
    *reinterpret_cast<INDTYPE*>(pstr)=nodeDists[nodePos].x; pstr+=sizeof(INDTYPE)/sizeof(uint32_t);
    *reinterpret_cast<INDTYPE*>(pstr)=nodeDists[nodePos].y; pstr+=sizeof(INDTYPE)/sizeof(uint32_t);
    *reinterpret_cast<float*>(pstr)=nodeDists[nodePos].dist; ++pstr;
  }
  *(uint32_t*)msg._str=i;
  msg._strlen=(sizeof(float)+2*sizeof(INDTYPE))*i+sizeof(uint32_t);
}

void nodeSendDistances(edcBaseClient& client)
{
  cerr << cnode << " sending starting from nodePos: " << nodePos<< endl;
  if (nodePos==-1l) return;

  long j;
  estr tmpdata;
  do {
    tmpdata.clear();
    nodeMakeDists(transfersize,tmpdata);
  } while (client.sendMsg(2,tmpdata) && nodePos>=0l);

}

long nodeComputeDistances(ebasicarray<long> _uniqind,long node,long tnodes,float thres,int _nthreads)
{
  load_seqs_compressed(argv[1],nodeArr,seqlen);

  nthreads=_nthreads;
  cerr << "# " << cnode << ") creating " << nthreads << " threads" << endl;
  taskman.createThread(nthreads);
  cnode=node;
  cerr << "# " << cnode << "/" << tnodes << ") computing distances. unique: " << _uniqind.size() << " total: "<< nodeArr.size() << endl;
  if ((long int)partsTotal>=((long int)nodeArr.size()-1l)*(long int)nodeArr.size()/(20l*tnodes))
    partsTotal=((long int)nodeArr.size()-1l)*(long int)nodeArr.size()/(20l*tnodes);

  long i;
  for (i=0; i<partsTotal; ++i){
    taskman.addTask(dfunc.value().calcfunc,evararray(mutexDists,_uniqind,nodeArr,nodeDists,(const int&)seqlen,(const long int&)(node*partsTotal+i),(const long int&)(partsTotal*tnodes),(const float&)thres));
  }
  taskman.wait();

  cerr << cnode << " finished computing distances: "<< nodeDists.size() << endl;

  mutexDists.lock();
  fradix256sort<eblockarray<eseqdist>,radixKey>(nodeDists);
//  nodeDists.sort();
  nodePos=nodeDists.size()-1l;
  cerr << cnode << " partsFinished: " << partsFinished << " partsTotal: " << partsTotal<< endl;
  cerr << cnode << " nodePos: " << nodePos<< endl;
  mutexDists.unlock();

  while (nodePos>=0l)
    nodeSendDistances(dcmpi);

  if (nodePos==-1l)
    cerr << "Sending EOF: " << dcmpi.sendMsg(3,"") << endl;
  cerr << cnode << " sending ended at nodePos: " << nodePos<< endl;

  dcmpi.final();
  exit(0);

  return(nodePos+1l);
}

estr args2str(int argvc,char **argv)
{
  estr tmpstr;
  int i;
  for (i=0; i<argvc; ++i)
    tmpstr+=estr(argv[i])+" ";
  tmpstr.del(-1);
  return(tmpstr);
}

estr host;

void doStartClient(edcBaseClient& client)
{
  int pipefd[2];
  ldieif(pipe(pipefd)!=0,"creating pipe");
}

void doStartServer(edcBaseServer& server)
{
  long i;
  load_seqs_compressed(argv[1],arr,seqlen);
#ifndef HPC_CLUST_USE_LONGIND
  ldieif(arr.size() > (2l<<31),"To cluster more than 2 million sequences please recompile hpc-clust with the --enable-longind flag.");
#endif
  ebasicstrhashof<long> duphash;
  ebasicstrhashof<long>::iter it;
  duphash.reserve(arr.size());
  earray<ebasicarray<INDTYPE> > dupslist;
  for (i=0; i<arr.size(); ++i){
    if (i%(arr.size()/10)==0)
      fprintf(stderr,"\r%li/%li",i,(long)arr.size());
    it=duphash.get(arr.values(i));
    if (it==duphash.end())
      { uniqind.add(i); duphash.add(arr.values(i),uniqind.size()-1); dupslist.add(ebasicarray<INDTYPE>(i)); }
    else 
      dupslist[it.value()].add(i);
  }
  randperm(uniqind);
  fprintf(stderr,"\r%li\n",(long int)arr.size());
  cout << endl;
  cout << "# unique seqs: " << uniqind.size() << endl;

  if (al)
    alcluster.init(arr.size(),ofile+".al",argv[1],dupslist,t,dfunc.value().calcfunc_single,arr,seqlen);
  if (cl)
    clcluster.init(arr.size(),ofile+".cl",argv[1],dupslist);
  if (sl)
    slcluster.init(arr.size(),ofile+".sl",argv[1],dupslist);
  if (dists)
    fdists.open(ofile+".dist","w");


  server.callbacks.add(serverRecvDistance);
  server.callbacks.add(serverFinished);

  serverStartComputation(server);
}

void help()
{
  printf("HPC-CLUST-MPI v%s\n",HPC_CLUST_PACKAGE_VERSION);
  printf("by Joao F. Matias Rodrigues and Christian von Mering\n");
  printf("Institute of Molecular Life Sciences, University of Zurich, Switzerland\n");
  printf("Matias Rodrigues JF, Mering C von. HPC-CLUST: Distributed hierarchical clustering for very large sets of nucleotide sequences. Bioinformatics. 2013.\n");
  printf("\n");
  printf("Usage:\n");
  printf("    %s [...] <-sl true|-cl true|-al true> aligned_seqs\n",efile(argv[0]).basename()._str);
  printf("\n");
  printf("Distributed hierarchical clustering of a set of multiple aligned sequences until a given threshold.\n");
  printf("Example: mpirun -n 20 hpc-clust-mpi -nthreads 1 -t 0.8 -dfunc gap -sl true myalignedseqs.sto\n");
  printf("   Will start 20 processes (with one thread each) to compute the clustering of the input file until 80%% id threshold\n");
  printf("\n");
  printf("Optional arguments:\n");
  printf("%10s    %s\n","-t","distance threshold until which to do the clustering [default: 0.9]");
  printf("%10s    %s\n","-dfunc","distance function to use: gap, nogap, tamura [default: gap]");
  printf("%10s    %s\n","-nthreads","number of threads to use [default: 1]");
  printf("%10s    %s\n","-ofile","output filename [defaults to input filename]. \".sl\",\".cl\", or \".al\" extensions will be appended");
  printf("\n");
  printf("At least one is required:\n");
  printf("%10s    %s\n","-sl true","perform single-linkage clustering");
  printf("%10s    %s\n","-cl true","perform complete-linkage clustering");
  printf("%10s    %s\n","-al true","perform average-linkage clustering");
  printf("\n");
  printf("Report bugs to: joao.rodrigues@imls.uzh.ch\n");
  printf("http://meringlab.org/software/hpc-clust/\n");

  exit(0);
}

int emain()
{
  dfunc.choice=0;
  dfunc.add("gap",edistfunc(t_calc_dists_u<estrarray,eseqdist,eblockarray<eseqdist>,dist_compressed2>,dist_compressed2));
  dfunc.add("nogap",edistfunc(t_calc_dists_u<estrarray,eseqdist,eblockarray<eseqdist>,dist_nogap_compressed2>,dist_nogap_compressed2));
  dfunc.add("gap2",edistfunc(t_calc_dists_u<estrarray,eseqdist,eblockarray<eseqdist>,dist_compressed>,dist_compressed));
  dfunc.add("nogap2",edistfunc(t_calc_dists_u<estrarray,eseqdist,eblockarray<eseqdist>,dist_nogap_compressed>,dist_nogap_compressed));
  dfunc.add("nogapsingle",edistfunc(t_calc_dists_u<estrarray,eseqdist,eblockarray<eseqdist>,dist_nogapsingle_compressed>,dist_nogapsingle_compressed));
  dfunc.add("tamura",edistfunc(t_calc_dists_u<estrarray,eseqdist,eblockarray<eseqdist>,dist_tamura_compressed>,dist_tamura_compressed));

  epregisterClass(eoption<edistfunc>);
  epregisterClassMethod4(eoption<edistfunc>,operator=,int,(const estr& val),"=");

  epregisterFunc(help);
  epregister(dfunc);


  epregister(dists);
  epregister(al);
  epregister(cl);
  epregister(sl);

  epregister(t);
  epregister(host);
//  epregister(ncpus);
  epregister(nthreads);
  epregister(tbucket);
  epregister(ofile);
  empiParseArgs(argvc,&argv);
  if(argvc<2) {
    cout << "syntax: "+efile(argv[0]).basename()+" <-sl true|-cl true|-al true> <seqali>" << endl;
    cout << "\""+efile(argv[0]).basename()+ " --help\" for more help" << endl;
    exit(-1);
  }
  if(!cl && !sl && !al) {
    cout << "syntax: "+efile(argv[0]).basename()+" <-sl true|-cl true|-al true> <seqali>" << endl;
    cout << "please choose at least one clustering method <-sl true|-cl true|-al true>" << endl;
    cout << "\""+efile(argv[0]).basename()+ " --help\" for more help" << endl;
    exit(-1);
  }

  cout << "# " << date() << endl;
  cout << "# " << args2str(argvc,argv) << endl;

//  initLookupTable();

  if (ofile.len()==0)
    ofile=argv[1];

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

  epregisterClass(ebasicarray<long>);
  epregisterClassInheritance(ebasicarray<long>,ebasearray);
  epregisterClassMethod(ebasicarray<long>,subset);
  epregisterClassSerializeMethod(ebasicarray<long>);

  epregisterClass(earray<long>);
  epregisterClassInheritance(earray<long>,ebasearray);
  epregisterClassMethod(earray<long>,subset);
  epregisterClassSerializeMethod(earray<long>);

  epregisterClassInheritance(edcmpi,edcBaseServer);
  epregisterClassInheritance(edcmpi,edcBaseClient);
  epregisterClassInheritance(edcmpiServerNode,edcBaseServerNode);

  epregisterFunc(nodeComputeDistances);

  cout << "# starting mpi cluster" << endl;
  dcmpi.onStartClient=doStartClient;
  dcmpi.onStartServer=doStartServer;

  cout << "# initializing MPI library" << endl;
  dcmpi.init(); //init and run

  dcmpi.final();

  return(0);
}

